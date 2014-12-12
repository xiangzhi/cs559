//CS559 - Project 2 - Xiang Zhi Tan
//normalMapping.h
//A class that seperates the normal mapping algorithms


#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <vector>

//given the vertexList, normal and uv, this method generates the tangents and bitangents
//thse tangets are used for normal mapping calculation in the shaders
//this is based on the tutorial from:
//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
void createNormalMapping(std::vector<glm::vec3>& vertexlist, std::vector<glm::vec3>& normallist, std::vector<glm::vec2>& uvlist,
  std::vector<glm::vec3> & tangents, std::vector<glm::vec3> & bitangents);