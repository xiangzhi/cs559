#pragma once

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <glm.hpp>
#include <vector>

class Loop
{
public:
  Loop();
  ~Loop();
};

void calculate();
std::vector<float> loopSubDivision(std::vector<float> list);
void loopTest(int numTimes, glm::mat4 proj, glm::mat4 view, glm::mat4 model);