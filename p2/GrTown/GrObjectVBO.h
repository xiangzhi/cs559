#ifndef GrObjectVBO_H
#define GrObjectVBO_H


#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "glm.hpp"
#include <gtx\transform.hpp>
#include <gtx\rotate_vector.hpp>
#include <vector>
#include "Utilities\Texture.H"
#include "Utilities\ShaderTools.H"
#include "DrawingTools.h"

//GrOBjectVBO
//the aim of the class is to run VBO code with a similar structure as as the old GrObject
//currently its just a direct clone


// other classes referred to:
class DrawingState;
class Behavior;

extern int notTheSame;
class GrObjectVBO {
public:
  GrObjectVBO(char* nameBase = "NoName", int& nameCtr2 = notTheSame, char* xn = 0); 
  GrObjectVBO(std::string name, int& nameCtr2 = notTheSame, char* xn = 0);
	virtual ~GrObjectVBO();


	//use to change stuff before drawing
   virtual void preDraw(DrawingState* drst);

  glm::vec3 translation;

	// things to know about the object
	bool ridable;					/* can we "ride" this thing */
	std::string name;
	float rideAbove;				// in case we don't want to be stuck on the ground

	// simulation: every object is required to "update itself" to the
	// current time. no guaruntees that this clock is updated
	// uniformly (since it depends on drawing rate). The units are
	// number of milliseconds since the beginning of the simulation
	virtual void simulateUntil(unsigned long time);

	// what makes the object go - by default, this is all that
	// SimulateUntil does.
	// note - this is a linked list! All behaviors get their chance
	std::vector<Behavior*> behaviors;

	// the main thing an object does is draw itself
	// this happens in 2 phases, one where normal drawing happens, and
	// one phase that is "after" all the other objects have draw. This
	// allows for transparent objects to be drawn last. 
	// The caller will always take care of doing a
	// pushAttributes/matrix, so don't worry about that
	virtual void draw(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
	virtual void drawAfter(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
	
	//this is initializing the stuff
	virtual void initialize();
	//really drawing it..
	virtual void realDraw(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);

	//VBO informations
	GLuint shaderID;
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
  GLuint indexBuffer;
  int indexNum;
  GLuint type = GL_TRIANGLES;
	Texture * t = NULL;
	int vertexNum;
  bool redraw = true;
  bool redrawAfter = true;
	bool useTexture = false;
  bool useIndex = false;
  glm::vec3 pos;
  glm::vec3 from;

  virtual void initializeAfter();


  virtual void runAttribute(glm::mat4 proj, glm::mat4 view, glm::mat4 model);
  virtual void runAttributeAfter();


	// most objects have some way to position themselves
	// by standardizing that, we can make any object "ridable"
	// in general, most objects will have their centers at their
	// local original and use this matrix to move them around
	glm::mat4 transform;

  //transform that won't effect the children;
  glm::mat4 localTransform;

	// if we are "riding" this object (that means that it is the current
	// camera), it has two responsibilities:
	// 1) it must set up the camera transformation (not the projection)
	// 2) it must provide the user interface
	//support for GLM
	virtual glm::mat4 getCamera();


	virtual bool uiStep();		// called if you're the camera before
	// drawing - lets you do key polling
	virtual int handle(int e);	// exactly what the FlTk Widgets do

	// we also have hierarchy
	GrObjectVBO* parent;				// your parent is responsible for you
	std::vector<GrObjectVBO*> children;
	GrObjectVBO* add(GrObjectVBO*, float x = 0, float y = 0, float z = 0, float ry = 0);

	// an object might be "interesting" - that is, that we want to put a
	// bookmark in the UI 
	// store the lookat and lookfrom points for these objects - in local
	// coordinates
	bool interesting;
	float laX, laY, laZ;
	float lfX, lfY, lfZ;
};

// add a new object onto the objects list
// allow for positioning in case object doesn't
// note: if you want to make a subobj, use the method of GrObj
extern std::vector<GrObjectVBO*> theVBOobjects;
GrObjectVBO* add(GrObjectVBO*, float x = 0, float y = 0, float z = 0, float ry = 0);

// draw a list of objects
// note: this has to recurse to draw each objects children!
void drawObList(std::vector<GrObjectVBO*>& head, DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
void drawAfterObList(std::vector<GrObjectVBO*>&, DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model);


// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/GrObjectVBO.H,v 1.4 2008/11/14 19:53:30 gleicher Exp $

#endif
