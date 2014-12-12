//CS 559 - Project 2 - Xiang Zhi Tan
//World.h
//This class was the evolution of the GrWorld given by Framework
//in charged of creating the world around



#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <gtx\rotate_vector.hpp>

#include <vector>

#include "DrawingState.H"
#include "DrawingTools.h"
#include "normalMapping.h"
#include "Utilities\Texture.H"
#include "Utilities\ShaderTools.H"
#include "SurfaceOfRevolution.h"
#include "GrTown_PCH.H"
#include "GrObjectVBO.h"


//draw a skyDome around the user.
void drawSkyBoxDome(glm::vec3 sun, float light, glm::mat4 PV, glm::vec3 camPos);

//draw the Earth and land
void drawEarth(glm::mat4 P, glm::mat4 V, glm::vec3 camPos, glm::vec3 sunDirection, float light);

//calculate how much sun light the world have right now
float calculateSunLight(DrawingState* drst);

//calculate the sun's direction
glm::vec3 calculateSunDirection(DrawingState* drst);

//copied from the function in the original GrWorld.cpp
//combined both of the world files
void drawSky(DrawingState* st);