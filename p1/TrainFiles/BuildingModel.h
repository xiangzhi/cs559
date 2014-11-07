#pragma once

/*
* Building Model.h
* Contains the drawing methods to draw the building
* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/


#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>
#include <glm.hpp>

#include "Utilities\Pnt3f.h"
#include <stdlib.h> 

//draw a building model with height;
void drawBuildingModel(int x, int z, float height);
