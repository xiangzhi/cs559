#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <gtx\rotate_vector.hpp>

#include "DrawingState.H"

#include "DrawingTools.h"


//draw the skybox
//just four side of a box with a top 
//pass in the direction of the sun and intensity of light
void drawSkyBox(glm::vec3 sun, float light, glm::mat4 MVP);

void drawSkyBoxDome(glm::vec3 sun, float light, glm::mat4 MVP);

void drawEarth(glm::mat4 P, glm::mat4 V, glm::vec3 camPos, glm::vec3 sunDirection, float light);

float calculateSunLight(DrawingState* drst);

glm::vec3 calculateSunDirection(DrawingState* drst);

//copied from the function in the original GrWorld.cpp
//combined both of the world files
void drawSky(DrawingState* st);