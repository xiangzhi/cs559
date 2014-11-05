#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <FL/Fl.h>
#include <GL/glu.h>
#include <glm.hpp>

#include "Utilities\Pnt3f.h"



/*
 * Draws the Train Models in OpenGL format
 * does not deal with orientation, exist in its own coordinate system
 */
class TrainModel{
	public:
		//draw the front car
		static void drawFrontCar(bool);
		static void drawBackCar(bool);
		static void drawMiddleCar(bool);
};

