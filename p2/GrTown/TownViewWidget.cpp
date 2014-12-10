/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005

#include "GrTown_PCH.H"

#include "TownViewWidget.H"
#include "DrawingState.H"
#include "GrWorld.H"
#include "GraphicsTownUI.H"
#include "GrObjectVBO.h"

#include "World.h"
#include "Loop.h"
#include "DrawUtils.H"
#include "Utilities/Texture.H"
#include "Utilities/ShaderTools.H"
#include <time.h>
#include "FlyCamera.H"

#include <glm.hpp>
#include <gtx\transform.hpp>
#include <iostream>
#include <vector>

#include "Particles.h"
#include "SurfaceOfRevolution.h"

using std::vector;

// at idle time, this gets called
// it must advance time to the current time step by simulating
// all of the objects
// it also gives the camera a chance to do its user interface
void tvIdler(void* v)
{
  TownViewWidget* tv = (TownViewWidget*) v;
  if (tv->getCamera()->uiStep())
	tv->damage(1);

  unsigned long t = clock();
  unsigned long dt = t- tv->lastClock;

  tv->lastClock = t;

  float speedup = static_cast<float>(tv->ui->speedup->value());

  unsigned long ti = static_cast<unsigned long>(static_cast<float>(dt) * speedup);

  tv->time +=  ti;

 
 
  if (ti > 0){

	  for (vector<GrObjectVBO*>::iterator g = theVBOobjects.begin(); g != theVBOobjects.end(); ++g)
		  (*g)->simulateUntil(tv->time);
	 
  }
  tv->damage(1);
}

////////////////////////////////////////////////////////////////////////////
TownViewWidget::TownViewWidget(int x, int y, int w, int h, 
							   const char* l)
  : Fl_Gl_Window(x,y,w,h,l),
	time(0),	// start time at the beginning
	lastClock(clock())
{
  // we will probably want them all...
  mode(FL_RGB | FL_DOUBLE | FL_DEPTH | FL_ALPHA /*| FL_STENCIL*/);
  Fl::add_idle(tvIdler,this);
  followCamera = new FollowCam();
  interestingCamera = new InterestingCam();
}
  
unsigned long lastDrawDone = 0;

void TownViewWidget::draw()
{
  // figure out how to draw
  static GLuint VertexArrayID;
  //some openGL settings for the first time
  static bool firstTime = false;
  if (!firstTime){
    //enable using new VBOs
		glewInit();
		firstTime = true;

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
	}
  

  DrawingState drst;
  getStateFromUI(&drst);
  
  glEnable(GL_TEXTURE_2D);
  /*
  if (drst.backCull) {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
  } else
	  glDisable(GL_CULL_FACE);

  glFrontFace(GL_CCW);
  */
  // set up the camera for drawing!
  glEnable( GL_DEPTH_TEST );
  glDepthFunc(GL_LESS);
  

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /*
  // we use blending for everything nowadays - there's little cost to having it on
  // NOTE: we avoid Z-writes if alpha is small, so transparent really is transparent
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glAlphaFunc(GL_GREATER,0.05f);
  glEnable(GL_ALPHA_TEST);
  */

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  Matrix camera;
  //drst.camera->getCamera(camera);
  glm::mat4 cam = drst.camera->getCamera();
  // compute the aspect ratio so we don't distort things
  double aspect = ((double)w()) / ((double)h());
  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 proj = glm::perspective(drst.fieldOfView, (float)aspect, 0.1f, 10000.0f);
  // Camera matrix
  glm::mat4 view = cam;

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 model = glm::mat4(1.0f);//glm::translate(glm::mat4(1.0f),glm::vec3(10,5,0));  // Changes for each model !
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 MVP = proj * view * model; // Remember, matrix multiplication is the other way around
  glm::mat4 VP = proj * view;


  glm::vec3 camPos(cam[0][3], cam[1][3], cam[2][3]);
  FlyCamera* flyCam = dynamic_cast<FlyCamera *>(drst.camera);
  if (flyCam != NULL){
    camPos = glm::vec3(flyCam->posX, flyCam->posY, flyCam->posZ);
  }



  //lighting calculations
  float light = calculateSunLight(&drst);
  //day time
  glm::vec3 sun = calculateSunDirection(&drst);

  glClearStencil(0);



  // the actual clearing goes on in the sky routine since its the only
  // thing that knows what color to make the sky
  drawSky(&drst);
  //drawEarth(&drst);
  //drawEarthNew(&drst,MVP);
  drawEarth(proj, view, camPos, sun, light);
  
  drawSkyBox(sun, light, MVP);

  //surfaceOfRevolutionTest(MVP, sun);



  //draw test
//  loopTest(2,proj,view,model,sun);

  //Draw VBO based objects
  drawObList(theVBOobjects, &drst, proj, view, model);
  drawAfterObList(theVBOobjects, &drst, proj, view, model);

 


  if (lastDrawDone) {
	  double ifr = ((double)CLOCKS_PER_SEC) / (double) (clock()-lastDrawDone+1);
	  ui->rate->value(ifr);
  }
  lastDrawDone = clock();

}

void TownViewWidget::getStateFromUI(DrawingState* st)
{
	st->timeOfDay = (int) ui->time->value();
	st->fieldOfView = (float) ui->fov->value();
	st->camera = getCamera();
	//st->backCull = ui->cull->value();
	//st->drGrTex = ui->lgTex->value();
	st->physics = (ui->Physic->value() != 0);
	st->eatable = (ui->Eatable->value() != 0);
	st->soRRotation = ui->sorRotation->value();
	st->loopDivisionTimes = (int)ui->loopDivision->value();
}

GrObjectVBO* TownViewWidget::getCamera()
{
  int p = ui->pickCamera->value();
  if (p) {
	if (ui->follower->value()) {
	  followCamera->following =  (GrObjectVBO*) ui->pickCamera->data(p);
	  return followCamera;
	}
	return (GrObjectVBO*) ui->pickCamera->data(p);
  } else {
	p = ui->pickInteresting->value();
	if (p) {
	  interestingCamera->focus = (GrObjectVBO*) ui->pickInteresting->data(p);
	  return interestingCamera;
	} else
	  return defaultCamera;
  } 
}

int TownViewWidget::handle(int e)
{
	switch(e) {
	case FL_SHOW:
		show();
		return 1;
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		return 1;
	default:
		int r = getCamera()->handle(e);
		if (r) damage(1);
		return r;
	};
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/TownViewWidget.cpp,v 1.6 2010/11/17 22:50:23 gleicher Exp $
