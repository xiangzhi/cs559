#pragma once

#include "TrainFiles\TrainView.H"
#include "World.H"
#include "TrainWindow.H"
#include "TrainModel.h"
#include <iostream>
#include <vector>

#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>
#include <glm.hpp>


#include "3DTools.h"
#include "TrackModel.h"
#include "DrawTools.h"

class Draw
{
public:
	Draw();
	~Draw();
	vector<float> drawTrack(TrainView *tv, bool doingShadow);
	Pnt3f drawTrain(TrainView *tv, bool doingShadow);
	vector<Pnt3f> getLookingPoints(TrainView *tv);
	vector<float> distanceList;
	vector<vector<float>> arcLengthTable;
private:
	std::vector<Pnt3f> trackPoints;
	
	float drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4);
};

