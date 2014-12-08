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
void loopSubDivision(std::vector<float>& vertexlist, std::vector<float>& normallist, std::vector<float>& uvlist);
void loopTest(int numTimes, glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::vec3 sun);
