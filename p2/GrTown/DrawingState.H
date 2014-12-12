#ifndef DRAWINGSTATE_H
#define DRAWINGSTATE_H
/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
// updated Fall, 2005
//

// "Drawing State" - a structure that tells objects how to draw
// themselves. 
// Objects need lots of info, since they are responsible for a lot
// about drawing themselves. The drawing state gives them as much info
// as possible so they can do things like cull themselves, do fake
// lighting if they want to, ...
class GrObjectVBO;

// the drawing state class itself
class DrawingState {
public:
  DrawingState();				// make a new one
  DrawingState(DrawingState&);	// copy an old one

  // time of day - military time (0 = midnight, 12=noon)
  int timeOfDay;

  // which object is the camera?
  GrObjectVBO* camera;

  float fieldOfView;

  // these are really bools, but since bools generate too many warnings...
  int backCull;
  int drGrTex;		// draw ground textures

  //whether physics is actve or not
  bool physics = false;
  //how detail is the surface of rotation
  double soRRotation = 0;
  //number of times the surface is subdivided
  int loopDivisionTimes = 0;
  //whether buildings can be destroyed
  bool eatable = false;
};

// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/DrawingState.H,v 1.2 2007/11/01 21:23:42 gleicher Exp $
#endif
