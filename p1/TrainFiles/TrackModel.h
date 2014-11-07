#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "Utilities\Pnt3f.h"
#include "DrawTools.h"
/*
* TrackModel.h
* Contains the drawing methods to draw the track
* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/


#include "Utilities\3DUtils.h"
#include "3DTools.h"

class TrackModel
{
public:
	/* draw dual rails, not drawn till drawit is called*/
	static void drawDual(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow);
	/* draw one rails, not drawn till drawIt is called*/
	static void drawSimple(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow);
	/* draw the rail tails*/
	static void drawRailTrails(Pnt3f, Pnt3f, Pnt3f);
	/* draw the tracks*/
	static void drawIt();
	/* clear the pointList, must done before drawing*/
	static void clearIt();
	/* to store the coordinate of tracks*/
	static std::vector<Pnt3f> pointList1;
	static std::vector<Pnt3f> pointList2;
};