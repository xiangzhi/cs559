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
	static void drawTrack(TrainView *tv, bool doingShadow);
	Pnt3f drawTrain(TrainView *tv, bool doingShadow);
	static Pnt3f drawTrainN(TrainView *tv, bool doingShadow);
	vector<Pnt3f> getLookingPoints(TrainView *tv);
	vector<float> distanceList;
	vector<vector<float>> arcLengthTable;
private:
	float drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4);
};



/* get the point on the track with the u and i */
Pnt3f getPointOnTrack(TrainView *tv, float u, int i);
/* get the orientation vector at point u with the starting point of i */
Pnt3f getOrientationVector(TrainView *tv, float u, int i);
/* get the directional vector from pt to the point with u and i*/
Pnt3f getDirectionVector(Pnt3f pt, TrainView *tv, float u, int i);
