//CS559 - Project 2 - Xiang Zhi Tan
//BHLight.h
//A class that creates a cone of light using surface of revolution



#pragma once
#include "GrObjectVBO.h"
class BHLights :
  public GrObjectVBO
{
public:
  BHLights();
  //it takes in the object it is tracking
  BHLights(GrObjectVBO* BH, GrObjectVBO* track);
  ~BHLights();

  GrObjectVBO* BH;
  GrObjectVBO* track;

  void initializeAfter();
  void preDraw(DrawingState* drst);
  void draw(DrawingState*, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
  void redoSOR();
  float rotation = 10;
};

