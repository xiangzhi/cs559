/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
//
#include "GrTown_PCH.H"

#include "FlyCamera.H"
#include <time.h>
#include <iostream>

#include <gtx\transform.hpp>
#include <gtx\rotate_vector.hpp>

#include <math.h>
/** Degrees to Radian **/
#define degreesToRadians( degrees ) ((float)( ( degrees ) / 180.0 * M_PI )))

/** Radians to Degrees **/
#define radiansToDegrees( radians ) ((float)(( radians ) * ( 180.0 / M_PI )))

int flyCamCount = 0;

FlyCamera::FlyCamera() : GrObjectVBO("FlyCamera",flyCamCount), 
  direction(0), pitch(0), posX(1000), posY(1000), posZ(1000),
  lastUItime(0), buttonDown(0)
{
  pos = glm::vec3(0);
  direction = 7;
    pitch = 0.5;
  // don't make this ridable, since its special
  //  ridable = true; 
}
void FlyCamera::draw(DrawingState*)
{
}

//a direct clone from the old getCamera by changing it to glm
glm::mat4 FlyCamera::getCamera(){

  glm::mat4 m(1.0f);
  glm::mat4 m2(1.0f);
  glm::mat4 m3(1.0f);
 
  m = glm::translate(m, glm::vec3(-posX, -posY, -posZ));
  m2 = glm::rotate(glm::mat4(1.0f),radiansToDegrees(-direction), glm::vec3(0, 1, 0));
  glm::mat4 c3 = m2;
  m3 = m2 * m;
  c3 = m3;
  m2 = glm::rotate(glm::mat4(1.0f), radiansToDegrees(pitch), glm::vec3(1, 0, 0));
  glm::mat4 c4 = m2;
  m = m2 * m3;
  return m;
}


int FlyCamera::handle(int e)
{
	static int stX=0, stY=0;
	static int lX=0, lY=0;

	switch(e) {
	case FL_PUSH:
		lX = stX = Fl::event_x();
		lY = stY = Fl::event_y();
		buttonDown = Fl::event_button();
		return 1;
	case FL_RELEASE:
		buttonDown = 0;
		return 1;
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		switch(Fl::event_key()) {
		case 'z':
			break;
		};
		return 1;
	case FL_DRAG:
		int mX = Fl::event_x();
		int mY = Fl::event_y();
		if (buttonDown) {
			switch(Fl::event_button()) {
	        case 1:
			case 3:
				float dx = static_cast<float>(mX - lX);
				float dy = static_cast<float>(mY - lY);
				if (fabs(dx) > fabs(dy)) {
					direction -=  0.01f * dx;												
				} else {										
					pitch += 0.01f * dy;
				}
				break;
			}
		}
		
		buttonDown = Fl::event_button();

		lX = mX; lY = mY;
		return 1;
	};
	return 0;
}

// to go forward, we need to now what the "forward" direction
// is - to find it out, look at the Z axis of the camera transform
void FlyCamera::forward(float a)
{
	glm::mat4 cam = getCamera();
	//getCamera(cam);

	pos.x -= a*cam[0][2];
	pos.y -= a*cam[1][2];
	pos.z -= a*cam[2][2];

	posX -= a*cam[0][2];
	posY -= a*cam[1][2];
	posZ -= a*cam[2][2];
}
// move the position in the LOCAL coordinates
// use the transform to know what that is
// but the transform really isn't used, so we have to get the camera
void FlyCamera::localTrans(float x,float y,float z)
{
	glm::mat4 cam = getCamera();
	//getCamera(cam);

	pos.x -= x*cam[0][0] + y*cam[0][1] + z*cam[0][2];
	pos.y -= x*cam[1][0] + y*cam[1][1] + z*cam[1][2];
	pos.z -= x*cam[2][0] + y*cam[2][1] + z*cam[2][2];

	posX -= x*cam[0][0] + y*cam[0][1] + z*cam[0][2];
	posY -= x*cam[1][0] + y*cam[1][1] + z*cam[1][2];
	posZ -= x*cam[2][0] + y*cam[2][1] + z*cam[2][2];
}



bool FlyCamera::uiStep()
{
  unsigned long uitime = clock();
  unsigned long te = uitime-lastUItime;
  lastUItime = uitime;

  float u = ((float) te) / 30.f;

  bool didSomething = false;

  if (buttonDown>1) {
	forward(3 * u); didSomething = true; }

  if (Fl::get_key(FL_Left)) {
	direction += .1f * u; didSomething = true; }
  if (Fl::get_key(FL_Right)) {
	direction -= .1f * u; didSomething = true; }
  if (Fl::get_key(FL_Up)) {
	pitch -= .1f * u; didSomething = true; }
  if (Fl::get_key(FL_Down)) {
	pitch += .1f * u; didSomething = true; }
  if (Fl::get_key(' ')) {
	forward(3 * u); didSomething = true; }
  if (Fl::get_key('x')) {
	forward(-3 * u); didSomething = true; }
  if (Fl::get_key(FL_KP+'8')) {
	localTrans(0,-u,0); didSomething = true; }
  if (Fl::get_key(FL_KP+'2')) {
	localTrans(0,u,0); didSomething = true; }
  if (Fl::get_key(FL_KP+'4')) {
	localTrans(u,0,0); didSomething = true; }
  if (Fl::get_key(FL_KP+'6')) {
	localTrans(-u,0,0); didSomething = true; }
  if (Fl::get_key(FL_KP+'5')) {
	localTrans(0,0,u); didSomething = true; }
  if (Fl::get_key(FL_KP+'0')) {
	localTrans(0,0,-u); didSomething = true; }

  if (Fl::get_key('d')) {
	localTrans(-u,0,0); didSomething = true; }
  if (Fl::get_key('a')) {
	localTrans(u,0,0); didSomething = true; } 
  if (Fl::get_key('s')) {
	forward(-3 * u); didSomething = true; }
  if (Fl::get_key('w')) {
	forward(3 * u); didSomething = true; }
  

  return didSomething;
}


///////////////////////////////////////////////////////////////
FollowCam::FollowCam() : GrObjectVBO("follower"), 
	following(0), followDistance(100), minY(15)
{
  ridable = 1;
  lastUItime = 0;
}

/*
// build a transformation that looks at the object that is being
// followed, and tries to keep a sane distance away
void FollowCam::getCamera(Matrix camera)
{
  /*
  float atX, atY, atZ;		// where we're looking at
  if (following) {
	  /*
	atX = following->transform[3][0];
	atY = following->transform[3][1];
	atZ = following->transform[3][2];

  } else {
	  std::cerr << "No object for followcam!";
	  return;
  }

  /*
  // get the old "from" point from the matrix
  float oldFromX = transform[3][0];
  float oldFromY = transform[3][1];
  float oldFromZ = transform[3][2];
 
  // compute the new from point
  float fromX, fromY, fromZ;

  // for now, make the distance be constant
  float dx = oldFromX - atX;
  float dy = oldFromY - atY;
  float dz = oldFromZ - atZ;
  float d = sqrt(dx*dx+dy*dy+dz*dz);

  if (d<1) {
	fromX = atX + 10;
	fromY = atY + 10;
	fromZ = atZ + 10;
  } else {
	  float ds = followDistance / d;
	  fromX = atX + (dx * ds);
	  fromY = atY + (dy * ds);
	  fromZ = atZ + (dz * ds);
  }
  if (fromY < minY) fromY = minY;

  lookatMatrix(fromX,fromY,fromZ, atX,atY,atZ, camera);

}
*/

glm::mat4 FollowCam::getCamera(){

  float atX, atY, atZ;
  if (following){
    atX = following->pos.x;//following->transform[3][0];
    atY = following->pos.y;// following->transform[3][1];
    atZ = following->pos.z;//transform[3][2];
  } else {
    std::cerr << "No object for followcam!";
    return glm::mat4(1.0F);
  }

  // get the old "from" point from the matrix
  static float oldFromX = transform[3][0];
  static float oldFromY = transform[3][1];
  static float oldFromZ = transform[3][2];

  // compute the new from point
  float fromX, fromY, fromZ;

  // for now, make the distance be constant
  float dx = oldFromX - atX;
  float dy = oldFromY - atY;
  float dz = oldFromZ - atZ;
  float d = sqrt(dx*dx + dy*dy + dz*dz);

  if (d<1) {
    fromX = atX + 10;
    fromY = atY + 10;
    fromZ = atZ + 10;
  }
  else {
    float ds = followDistance / d;
    fromX = atX + (dx * ds);
    fromY = atY + (dy * ds);
    fromZ = atZ + (dz * ds);
  }
  if (fromY < minY) fromY = minY;


  oldFromX = fromX;
  oldFromY = fromY;
  oldFromZ = fromZ;

  return glm::lookAt(
    glm::vec3(fromX, fromY, fromZ),
    glm::vec3(atX, atY, atZ),
    glm::vec3(0, 1, 0)
  );
}


bool FollowCam::uiStep()
{  unsigned long uitime = clock();
  unsigned long te = uitime-lastUItime;
  lastUItime = uitime;

  float u = ((float) te) / 20.f;

  return true;
}

int FollowCam::handle(int e)
{
	switch(e) {
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		return 1;
	};
	return 0;
}

////////////////////////////////////////////////////////////////////////
Map::Map() :  GrObjectVBO("Map"), x(0), y(0), z(-2000)
{
  ridable = 1;
}
/*
void Map::getCamera(Matrix camera)
{
  transMatrix(camera,x,y,z);
  camera[0][0] = 1;
  camera[1][1] = 0;
  camera[1][2] = 1;
  camera[2][1] = -1;
  camera[2][2] = 0;
}
*/

glm::mat4 Map::getCamera(){
	glm::mat4 camera = glm::translate(glm::mat4(1.0f),glm::vec3(x, y, z));
	camera[0][0] = 1;
	camera[1][1] = 0;
	camera[1][2] = 1;
	camera[2][1] = -1;
	camera[2][2] = 0;
	return camera;
}

int Map::handle(int e)
{
	switch(e) {
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		switch(Fl::event_key()) {
		case ' ':
			x = 0;
			y = 0;
			z = -2000;
			break;
		};
		return 1;
	}
	return 1;
}

bool Map::uiStep()
{
  unsigned long uitime = clock();
  unsigned long te = uitime-lastUItime;
  lastUItime = uitime;

  float u = ((float) te) / 30.f;

  bool didSomething = false;

  const float s = 50.0f;

  if (Fl::get_key(FL_Left)) {
	x -= s * u; didSomething = true; }
  if (Fl::get_key(FL_Right)) {
	x += s * u; didSomething = true; }
  if (Fl::get_key(FL_Up)) {
	y += s * u; didSomething = true; }
  if (Fl::get_key(FL_Down)) {
	y -= s * u; didSomething = true; }

  return didSomething;
}

#include "glm.hpp"
#include <gtc\matrix_transform.hpp>
///////////////////////////////////////////////////////////////////////
InterestingCam::InterestingCam() : focus(0)
{
}

glm::mat4 InterestingCam::getCamera()
{
	// find the points in world coordinates
	if (focus) {
		return glm::lookAt(
			focus->from,
			focus->to,
			glm::vec3(0, 1, 0)
		);
	}
	return glm::lookAt(
		glm::vec3(100,100,100),
		glm::vec3(0,0,0),
		glm::vec3(0, 1, 0)
	);
	
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/FlyCamera.cpp,v 1.7 2007/11/12 16:56:06 gleicher Exp $
