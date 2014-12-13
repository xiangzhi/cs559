//CS559 - Project 2 - Xiang Zhi Tan
//Bob.h
//A monster that jumps aronund


#include "GrObjectVBO.h"
#include "Loop.h"
#include "DrawingState.H"

#pragma once
class Bob : public GrObjectVBO
{
public:
  Bob();
  ~Bob();
  int divideTime = 3;
  void initialize();
  virtual void simulateUntil(unsigned long time);
  void preDraw(DrawingState* drst);
  void redoSubDivision();
  glm::mat4 getCamera();
  DrawingState* drstInternal;
  //behavior attributes
  bool fall = false;;
  glm::vec3 acc;
  glm::vec3 scale;
  bool physic;
  glm::vec3 touchPos;

  //keep a copy of the original size;
  std::vector<glm::vec3> oriVertexList;
  std::vector<glm::vec3> oriNormalList;
  std::vector<glm::vec2> oriUvList;

};

