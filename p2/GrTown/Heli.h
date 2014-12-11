#pragma once
#include "GrObjectVBO.h"
#include "SurfaceOfRevolution.h"

class Heli :
  public GrObjectVBO
{
public:
  Heli();
  Heli(GrObjectVBO* track);
  ~Heli();
  void initialize();
  void preDraw(DrawingState* drst);
  glm::mat4 getCamera();
  GrObjectVBO* track;
  void simulateUntil(unsigned long time);
};

