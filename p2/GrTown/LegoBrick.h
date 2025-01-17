//CS559 - Project 2 - Xiang Zhi Tan
//LegoBrick.h
//A class that creates an individual lego brick


#pragma once
#include "GrObjectVBO.h"
#include "SurfaceOfRevolution.h"
#include "Loop.h"
class LegoBrick :
	public GrObjectVBO
{
public:
  LegoBrick();
  LegoBrick(float t);
	~LegoBrick();
	void initialize();
	void preDraw(DrawingState* drst);
  LegoBrick* clone();
  float height = 1;
  void simulateUntil(unsigned long time);

  //information about subdivision
  void doSubdivision();
  int divideTimes = 2;
  int color;
  //simulation attributes
  bool destroyed = false;
  bool doneDestroyed = false;
  long lastSimulated = 0;

  //a static list that will share the values across all instances
  //this is to decrease the time we do computation on it.
  static int vertexloopList[5];
  static int normalloopList[5];
  static int uvloopList[5];
  static int  vNumLoopList[5];
};