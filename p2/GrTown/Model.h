#pragma once
#include "GrObjectVBO.h"
class Model :
  public GrObjectVBO
{
public:
  Model();
  Model(GrObjectVBO* track);
  ~Model();
  void initialize();
  void preDraw(DrawingState* drst);
  glm::mat4 getCamera();
  GrObjectVBO* track;
};

