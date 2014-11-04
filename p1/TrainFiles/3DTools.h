#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "Utilities\Pnt3f.h"

/*
 * set a rotation matrix that rotate the object towards the dirPt
 * and has an up vector of orPt
*/
void alignObjectIn3D(Pnt3f dirPt, Pnt3f orPt);