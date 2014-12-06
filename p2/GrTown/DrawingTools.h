#pragma once
//bunch of useful short hand functions
//since I keep retyping the wrong functions

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>

//run the same binding method for everything
void bindToArrayBuffer(GLuint& ptrToBuffer, double sizeOfLocation, void* ptrToLocation);