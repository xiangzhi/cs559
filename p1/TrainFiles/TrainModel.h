#pragma once
/*
* trainMode.h
* Draws the Train Models in OpenGL format
* does not deal with orientation, exist in its own coordinate system

* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/

#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>
#include <glm.hpp>

#include "Utilities\Pnt3f.h"


class TrainModel{
	public:
		//draw the front car
		static void drawFrontCar(bool);
		//draw the middle car
		static void drawBackCar(bool);
		//draw the end car
		static void drawMiddleCar(bool);
};

