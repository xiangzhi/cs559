#pragma once
#include "GrObjectVBO.h"
#include "ObjectLoader.h"
#include "Loop.h"

class APC :
  public GrObjectVBO
{
public:
  APC();
  APC(float numTimes);
  ~APC();
  void initialize();
  void preDraw(DrawingState* drst);
  int times = 0;

};

