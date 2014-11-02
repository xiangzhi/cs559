#pragma once

#include "TrainFiles\TrainView.H"
#include "World.H"
#include "TrainWindow.H"
#include <iostream>

#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>

class Draw
{
public:
	Draw();
	~Draw();
	void drawTrack(TrainView *tv, bool doingShadow);
	void drawTrain(TrainView *tv, bool doingShadow);
private:
	std::vector<Pnt3f> trackPoints;
	void drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4);
};

