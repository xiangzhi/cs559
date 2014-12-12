//CS 559 - Project 2 - Xiang Zhi Tan
//Loop.h
//Do the loop algorithm for the object

#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <vector>

//do the subdivision for the object with the vertex,normal and uv for one time
void loopSubDivision(std::vector<float>& vertexlist, std::vector<float>& normallist, std::vector<float>& uvlist);
//do the subdivision for the object with the vertex,normal and uv for number of times given
void loopSubDivision(int times, std::vector<glm::vec3>& vertexlist, std::vector<glm::vec3>& normallist, std::vector<glm::vec2>& uvlist);
