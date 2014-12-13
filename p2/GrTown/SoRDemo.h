#pragma once
#include "GrObjectVBO.h"
class SoRDemo :
  public GrObjectVBO
{
public:
  SoRDemo();
  SoRDemo(float);
  ~SoRDemo();
  void initialize();
  void preDraw(DrawingState* drst);
  float rotation;

};

