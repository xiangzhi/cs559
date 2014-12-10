#pragma once
#include "GrObjectVBO.h"
class BHLights :
  public GrObjectVBO
{
public:
  BHLights();
  BHLights(GrObjectVBO* BH, GrObjectVBO* track);
  ~BHLights();

  GrObjectVBO* BH;
  GrObjectVBO* track;

  void initializeAfter();
  void preDraw(DrawingState* drst);
  void redoSOR();
  float rotation = 10;
};

