//CS559 - Project 2 - Xiang Zhi Tan
//Heli.h
//A class that loads a helicopter object and also the blades on top


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

