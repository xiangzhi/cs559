#pragma once
#include "GrObjectVBO.h"


class Billboards :
  public GrObjectVBO
{
public:
  Billboards();
  ~Billboards();

  void initialize();
  void realDraw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
};

