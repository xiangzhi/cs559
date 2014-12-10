#pragma once
#include "GrObjectVBO.h"
class City :
  public GrObjectVBO
{
public:
  City();
  ~City();
  GrObjectVBO* destroyer;
  void initialize();
  void preDraw(DrawingState* drst);
  void realDraw();
  void simulateUntil(unsigned long time);
};

