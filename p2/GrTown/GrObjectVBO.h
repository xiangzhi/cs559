//CS 559 - Project 2 - Xiang Zhi Tan
//GrObjectVBO.h
//A direct clone of the original GrObject.h
//it still have old members due to possible legacy code that might break


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
#include "DrawingState.H"


// other classes referred to:
class DrawingState;

extern int notTheSame;
class GrObjectVBO {
public:
  GrObjectVBO(char* nameBase = "NoName", int& nameCtr2 = notTheSame, char* xn = 0); 
  GrObjectVBO(std::string name, int& nameCtr2 = notTheSame, char* xn = 0);
	virtual ~GrObjectVBO();


	//use to change stuff before drawing
   virtual void preDraw(DrawingState* drst);



	// original data members
	bool ridable;					/* can we "ride" this thing */
	std::string name;
	float rideAbove;				// in case we don't want to be stuck on the ground

  //additional data members
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
  bool onlyBack = false;
  //transformation information for the model
  glm::vec3 pos;  //the position of the model
  glm::vec3 scale; // the scale of the model
  float ry = 0; //rotation around Y-axis

  //lookAt information
  glm::vec3 from; //where the eye is looking from at interest cam
  glm::vec3 to;

  //information if there is also a after writer
  GLuint shaderIDAfter;
  GLuint vertexBufferAfter;
  GLuint textureBufferAfter;
  GLuint normalBufferAfter;
  GLuint colorBufferAfter;
  int vertexNumAfter;

  // A method that clones the class and return it
  //if no pointer is given, a new class is made
  GrObjectVBO* clone(GrObjectVBO* ptr = nullptr);

	// simulation: every object is required to "update itself" to the
	// current time. no guaruntees that this clock is updated
	// uniformly (since it depends on drawing rate). The units are
	// number of milliseconds since the beginning of the simulation
	virtual void simulateUntil(unsigned long time);

	// the main thing an object does is draw itself
	// this happens in 2 phases, one where normal drawing happens, and
	// one phase that is "after" all the other objects have draw. This
	// allows for transparent objects to be drawn last. 
	// The caller will always take care of doing a
	// pushAttributes/matrix, so don't worry about that
	virtual void draw(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
	virtual void drawAfter(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
	

	//initializing it for VBO
  //rewrite it for each class as this is loaded
  //the first time during each draw
	virtual void initialize();
  virtual void initializeAfter();



  //Add more attribute to the drawing method if are not the defaults
  //one for first draw, one for the second draw
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
