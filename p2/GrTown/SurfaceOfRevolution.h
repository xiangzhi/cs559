//CS559 - Project 2 - Xiang Zhi Tan
//SurfaceOfRevoluation


#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>

#include <vector>



//pass the value in and rotate around the surface OfRevoultion.
//also return a list of uv and normals
void surfaceOfRevolution(glm::vec3 center, float sample, std::vector<glm::vec3>& vertexList, std::vector<glm::vec3>& normalList, std::vector<glm::vec2>& uvList);


