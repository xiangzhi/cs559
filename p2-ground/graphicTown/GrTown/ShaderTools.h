#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//taken straight from the tutorial on the website 
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);