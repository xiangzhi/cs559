#pragma once

/*
 * draw.h
 * main drawing file for the assignment
 * draws everything from tracks to train
 * does all the calcuations
 * Xiang Zhi Tan - CS 559 - Fall 2014
 */

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

//call back for interval drawing
typedef void(*drawCallback)(Pnt3f pt, Pnt3f dirPt, Pnt3f orPt);

class Draw
{
public:
	//draw track
	static void drawTrack(TrainView *tv, bool doingShadow);
	//draw the train
	static vector<Pnt3f> drawTrain(TrainView *tv, bool doingShadow);
	//get where to look at
	static vector<Pnt3f> getLookingPoints(TrainView *tv);
	// list of the curves
	vector<float> distanceList;
	// arc Length Table
	vector<vector<float>> arcLengthTable;
	float drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4);
};

