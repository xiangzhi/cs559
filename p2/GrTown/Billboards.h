#pragma once
#include "GrObjectVBO.h"


class Billboards :
  public GrObjectVBO
{
public:
  Billboards();
  ~Billboards();

  void initialize();
  void runAttribute(glm::mat4 proj, glm::mat4 view, glm::mat4 model);
  //void realDraw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model);
};

