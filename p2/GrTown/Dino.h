#pragma once
#include "GrObjectVBO.h"
#include "Loop.h"
class Dino :
  public GrObjectVBO
{
public:
  Dino();
  ~Dino();

  void initialize();
  void simulateUntil(unsigned long t);
  void preDraw(DrawingState* drst);
  float lx = 0;
  float lz = 0;
  float ax = 1;
  float az = 1;
  float dz = 1;
  float dx = 1;
  float rotation = 0;
};

