#pragma once
//bunch of useful short hand functions
//since I keep retyping the wrong functions

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>

#include <vector>

//run the same binding method for everything
void bindToArrayBuffer(GLuint& ptrToBuffer, double sizeOfLocation, void* ptrToLocation);
void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<float> list);
void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec3> list);
void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec2> list);
//rebind something
void rebindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec3> list);
void rebindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec2> list);

//return a list of normal and vertex that represent a cube with the variables
void drawCube(std::vector<glm::vec3> &vertexList, std::vector<glm::vec3> &normalList, std::vector<glm::vec2> &uvList, float _l = 10, float _w = 10, float _h = 10);
void drawCube(std::vector<float> &vertexList, std::vector<float> &normalList, float l = 10,float w = 10, float h = 10);


void draw3DRect(std::vector<float> &vertexList, std::vector<float> &normalList, float l1 = 10, float l2 = 10, float w1 =10, float w2 = 10, float h = 10);
//
void getTextureList(int size, std::vector<float> &uvList, int max = 1);


void listToVector(float * list, int size, std::vector<float>& vList);
void listToVector(float * list, int size, std::vector<glm::vec3>& vList);
void listToVector(float * list, int size, std::vector<glm::vec2>& vList);