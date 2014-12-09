
#include "GrObjectVBO.h"
#include "Loop.h"

#pragma once
class Bob : public GrObjectVBO
{
public:
  Bob();
  ~Bob();
  int divideTime = 3;
  void initialize();
  virtual void simulateUntil(unsigned long time);
  void preDraw();
  bool fall = false;;
  glm::vec3 acc;
  glm::vec3 scale;
};

