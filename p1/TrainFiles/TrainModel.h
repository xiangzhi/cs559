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
void drawTrainModel();

//draw the front car
void drawFrontCar();
void drawBackCar();
void drawMiddleCar();

