// CS559 Utility Code: Shader Tools
// a collection of useful routines for making use of Shaders
//
// written November 2008 by Michael Gleicher
// with code borrowed from the tutorial by Cody Robson
//
//Update 2014: Heavily changed to suit my need, based on code on the tutorial on line
// at http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

#include <string>
#include <vector>

//a different shader loader that is based on the one given in the following tutorial
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
//it loads the shader and if there is an error in the shader, it throws a runtime exception
//with the error(good for debug and well, you know the shader is wrong).
unsigned int loadShdaer(std::string vertexFileName, std::string fragmentFileName);

//leaved as legacy code, the error never returns error
unsigned int loadShader(const char* vertexFileName, const char* fragmentFileName,
				        char*& error);

// we'll look for shader programs on this list of paths
extern std::vector<std::string> shaderPaths;

#define _USE_MATH_DEFINES
// $Header: /p/course/cs559-gleicher/private/CVS/Utilities/ShaderTools.H,v 1.3 2008/11/11 04:27:57 gleicher Exp $
