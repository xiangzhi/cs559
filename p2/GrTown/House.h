#pragma once
#include "GrObjectVBO.h"
#include "Bob.h"
class House :
	public GrObjectVBO
{
public:
	House();
	~House();
  void preDraw(DrawingState* drst);
  bool destroyed = false;
  bool doneDestroyed = false;
  bool isOver(Bob*);
  void simulateUntil(unsigned long time);
  //share the vertex pointers across the same object
  static int vertexloopList[5];
  static int normalloopList[5];
  static int uvloopList[5];
  static int  vNumLoopList[5];
  int divideTime = 2;
  long lastSimulation = 0;
  std::vector<glm::vec3>* vertexList;
  std::vector<glm::vec3>* normalList;
  std::vector<glm::vec2>* uvList;
  void redoSubDivision();
  bool sameSpace(House * destroyer);
  int color = -1;
  int kills = 0;
};

