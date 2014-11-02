#pragma once

#include "TrainFiles\TrainView.H"
#include "World.H"
#include "TrainWindow.H"
#include <iostream>
#include <vector>

#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>
#include <glm.hpp>

class Draw
{
public:
	Draw();
	~Draw();
	vector<float> drawTrack(TrainView *tv, bool doingShadow);
	void drawTrain(TrainView *tv, bool doingShadow);
	vector<Pnt3f> getLookingPoints(TrainView *tv);
	vector<float> distanceList;
private:
	std::vector<Pnt3f> trackPoints;
	
	float drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4);
};

