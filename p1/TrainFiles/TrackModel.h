#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Utilities\Pnt3f.h"
#include "DrawTools.h"
#include "Utilities\3DUtils.h"


class TrackModel
{
public:
	TrackModel();
	~TrackModel();
	static void draw(Pnt3f p1, Pnt3f p2, bool shadow);
	static void drawDual(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow);
	static void drawTrackBottom(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow);
};