//CS559 - Project 2 - Xiang Zhi Tan
//City.h
//A class that generates the lego blocks and put them into place


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

