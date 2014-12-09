// CS559 Utility Code: Shader Tools
// a collection of useful routines for making use of Shaders
//
// written November 2008 by Michael Gleicher
// with code borrowed from the tutorial by Cody Robson
//
// Update 2014: change GLEE to GLEW
// warning: this code assumes that you're using GLEW
//
//Update 2014: Heavily changed to suit my need, based on code on the tutorial on line
// at http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

#define GLEW_STATIC 1

#include <windows.h>
#include <GL/glew.h>

#include "ShaderTools.H"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>

using std::ifstream;
using std::string;
using std::vector;

vector<string> shaderPaths;


//method that check whether the file exist of not
//based on the answer given at
//http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
inline bool fileExistsTest(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//loop through the list of paths and find the shader,
//return empty if cannot find.
std::string findCorrectPath(std::string& name){

	std::string path = "";

	for (int i = 0; i < shaderPaths.size(); i++){
		std::string temp = shaderPaths[i] + "/" + name;
		if (fileExistsTest(temp)){
			path = temp;
			break;
		}
	}
	return path;
}

//a different shader loader that is based on the one given in the following tutorial
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
GLuint loadShader(std::string &vertexFileName, std::string& fragmentFileName){
	//find the correct path to the vertex shader
	std::string vertex_file_path = findCorrectPath((std::string)vertexFileName);
	//if empty string means cannot find it.
	if (vertex_file_path.size() == 0){
		throw std::runtime_error("cannot find vertex shader " + (std::string)vertexFileName);
	}
	//find the correct path to the vertex shader
	std::string fragment_file_path = findCorrectPath((std::string)fragmentFileName);

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		//this is total failure, means the file doesn't exist
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
  std::cout << "Compiling Vectex shader : " << vertex_file_path << std::endl;
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	if (VertexShaderErrorMessage.size() > 1){
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stderr, "ERROR:%s\n", &VertexShaderErrorMessage[0]);
		throw std::runtime_error(&VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
  std::cout << "Compiling Fragment shader : " << fragment_file_path << std::endl;
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	if (FragmentShaderErrorMessage.size() > 1){
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		//straight away throw error
    fprintf(stderr, "ERROR:%s\n", &FragmentShaderErrorMessage[0]);
		throw std::runtime_error(&FragmentShaderErrorMessage[0]);
	}

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(InfoLogLength);
	if (ProgramErrorMessage.size() > 1){
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stderr, "ERROR:%s\n", &FragmentShaderErrorMessage[0]);
		throw std::runtime_error(&FragmentShaderErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	fprintf(stdout, "SharderID: %d\n", (int)ProgramID);
	return ProgramID;
}


//shortHand
GLuint loadShader(const char* vertexFileName, const char* fragmentFileName){
  return loadShader((std::string)vertexFileName, (std::string)fragmentFileName);
}


//leaved for legacy codes that might used this.
GLuint loadShader(const char* vertexFileName, const char* fragmentFileName, char*& error){
  //redirect to actuall code based on the tutorial
  return loadShader((std::string)vertexFileName, (std::string)fragmentFileName);
}