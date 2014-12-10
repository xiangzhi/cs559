
#include "GrObjectVBO.h"
#include "Loop.h"
#include "DrawingState.H"

#pragma once
class Bob : public GrObjectVBO
{
public:
  Bob();
  ~Bob();
  int divideTime = 3;
  void initialize();
  virtual void simulateUntil(unsigned long time);
  void preDraw(DrawingState* drst);
  void redoSubDivision();
  glm::mat4 getCamera();
  bool fall = false;;
  glm::vec3 acc;
  glm::vec3 scale;
  bool physic;
  glm::vec3 touchPos;
};

