/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
//
#include "GrTown_PCH.H"

#include "GrWorld.H"
#include "DrawingState.H"
#include "DrawUtils.H"
#include "Utilities\ShaderTools.H"
#include "Utilities\Texture.H"
#include "GrObjectVBO.h"

using std::vector;

// stuff in the world
vector<GrObject*> theObjects;
vector<GrObjectVBO*> theVBOobjects;

// things for drawing the world

// remember, these function have some responsibilities defined in the
// header... 
// for now these do simple things. in the future, maybe they will draw
// more nicely
void drawSky(DrawingState* st)
{
  // figure out the sky color...
  if (st->timeOfDay < 5) st->sky(0,0,.2f);
  else if (st->timeOfDay < 8) st->sky(.2f,.2f,.8f);
  else if (st->timeOfDay < 16) st->sky(.7f,.7f,1);
  else if (st->timeOfDay < 19) st->sky(.2f,.2f,.8f);
  else st->sky(0,0,.2f);

  glClearColor(st->sky.r,st->sky.g,st->sky.b,st->sky.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
}

// for now, just draw a huge green square. 10 miles on a side (5000
// ft/mile) 
void drawEarth(DrawingState* st)
{
 
  st->ground(0,84,24);

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.,10);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glColor4fv(&st->ground.r);
  glVertex3d(-25000,-1, 25000);
  glVertex3d( 25000,-1, 25000);
  glVertex3d( 25000,-1,-25000);
  glVertex3d(-25000,-1,-25000);
  glEnd();
  glDisable(GL_POLYGON_OFFSET_FILL);

  }


void drawEarthNew(DrawingState* st, glm::mat4 MVP)
{

  static GLuint vertexBuffer;
  static GLuint shaderId;
  static GLuint textureId;
  static GLuint textureBuffer;
  static Texture* t;
  static bool firstTime = false;

  if (!firstTime){
    firstTime = true;

    //all the vertexs
    float points[] = {
      -25000.0f, -1.0f, -25000.0f,
      25000.0f, -1.0f, -25000.0f,
      25000.0f, -1.0f, 25000.0f,
      -25000.0f, -1.0f, -25000.0f,
      -25000.0f, -1.0f, 25000.0f,
      25000.0f, -1.0f, 25000.0f
    };

    float UVpoints[] = {
      0.0f, 0.0f,
      1000.0f, 0.0f,
      1000.0f, 1000.0f,
      0.0f, 0.0f,
      0.0f, 1000.0f,
      1000.0f, 1000.0f
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

    char* err;
    shaderId = loadShader("EarthVertex.glsl", "EarthFragment.glsl", err);

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof (UVpoints), UVpoints, GL_STATIC_DRAW);

    t = fetchTexture("grass2.jpg", true, true);
    textureId = t->texName;
    
  }

  t->bind();
  



  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );


  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  //bind color buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glVertexAttribPointer(
	  2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	  3,                                // size
	  GL_FLOAT,                         // type
	  GL_FALSE,                         // normalized?
	  0,                                // stride
	  (void*)0                          // array buffer offset
	  );
  //get



  GLuint sampleLoc = glGetUniformLocation(shaderId, "textureInput");
  glUniform1i(sampleLoc, textureId);

  glUseProgram(shaderId);




  // Get a handle for our "MVP" uniform.
  // Only at initialisation time.
  GLuint MatrixID = glGetUniformLocation(shaderId, "MVP");
  // Send our transformation to the currently bound shader,
  // in the "MVP" uniform
  // For each model you render, since the MVP will be different (at least the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  // draw points 0-3 from the currently bound VAO with current in-use shader

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  //unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
}



// 
// setup lighting
void setupLights(DrawingState* dr)
{
  // depending on time of day, the lighting changes
  // ambient is either night or day
  if ((dr->timeOfDay >= 5) && (dr->timeOfDay <=19)) {
	float a0[] = {.4f,.4f,.4f,.4f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, a0);
  } else {
	float a1[] = {.2f,.2f,.2f,.2f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, a1);
  }
  // directional, if its on, depends on what hour it is
  float pos[4] = {0,0,0,0};
  if ((dr->timeOfDay >= 5) && (dr->timeOfDay <=19)) {
	float angle = (((float)(dr->timeOfDay-5)) / 7.f) * (3.14159f/2.f);
	pos[0] = (float) cos(angle);
	pos[1] = (float) sin(angle);
  }
  else {
	  pos[1] = -1;
  }
  glLightfv(GL_LIGHT0, GL_POSITION, pos);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
}

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/GrWorld.cpp,v 1.3 2008/11/11 03:48:23 gleicher Exp $
