

#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <glm.hpp>
#include "stdlib.h"
#include "stdio.h"


#include <stdio.h>
#include <direct.h>
#include <vector>


bool loadOBJ(
  const char * path,
  std::vector < glm::vec3 > & out_vertices,
  std::vector < glm::vec2 > & out_uvs,
  std::vector < glm::vec3 > & out_normals
  );