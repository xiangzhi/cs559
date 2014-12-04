// CS559 Utility Code: Shader Tools
// a collection of useful routines for making use of Shaders
//
// written November 2008 by Michael Gleicher
// with code borrowed from the tutorial by Cody Robson
//
// Update 2014: change GLEE to GLEW
// warning: this code assumes that you're using GLEW

#define GLEW_STATIC 1

#include <windows.h>
#include <GL/glew.h>



#include "ShaderTools.H"
#include "stdlib.h"
#include "stdio.h"


#include <stdio.h>
#include <direct.h>


#include <fstream>

using std::ifstream;
using std::string;
using std::vector;

vector<string> shaderPaths;

// read in a file once you have the file name - called from the real routine
// that searches in a path
static bool readShaderFileI(const char* filename, std::vector<std::string>& program)
{

	program.clear();
    ifstream inFile(filename);
	if (! inFile.good()) return false; 
    string temp;
    while (getline(inFile, temp)) {
        program.push_back(temp);
    }
	return true;
}

// look for a shader file in the shaderPaths (try no path first)
bool readShaderFile(const char* filename, std::vector<std::string>& program)
{
	// try no path first
	if (readShaderFileI(filename,program)) return true;

	// otherwise, try all the paths
	for(vector<string>::iterator i=shaderPaths.begin(); i != shaderPaths.end(); ++i) 
	{
		string fullpath = *i + "/" + filename;
		if (readShaderFileI(fullpath.c_str(),program)) return true;
	}
	return false;
}

// see if the shader compiled correctly
// if it did not, return a true value, and print an error
bool checkShaderError(GLuint shader)
{
	GLint compiled;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
	if (compiled)
		return false;
	else {
		GLint length;
		GLchar* log;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);
		log = (GLchar*) malloc(length+1);
		glGetShaderInfoLog(shader,length,&length,log);
		printf("Shader Compilation Log:\n%s\n",log);
		free(log);
		return true;
	}
}

// see if the program linked correctly
// if it did not, return a true value, and print an error
bool checkProgramError(GLuint program)
{
	GLint compiled;
	glGetProgramiv(program,GL_LINK_STATUS,&compiled);
	if (compiled)
		return false;
	else {
		GLint length;
		GLchar* log;
		glGetProgramiv(program,GL_INFO_LOG_LENGTH,&length);
		log = (GLchar*) malloc(length+1);
		glGetProgramInfoLog(program,length,&length,log);
		printf("Shader Compilation Log:\n%s\n",log);
		free(log);
		return true;
	}
}

// for reasons I don't understand, this program barfs if there is
// a C++ style comment - they're easy enough to get rid of, so rather than
// sorting it out, here's something easier
void ridCPPcomments(vector<string>& prog)
{
	for(vector<string>::iterator i=prog.begin(); i != prog.end(); ++i) {
		string::size_type f = i->find("//");
		if (f != string::npos) 
			i->erase(i->begin()+f,i->end());
	}
}

GLuint loadShader(const char* vertexFileName, const char* fragmentFileName, char*& error){

  char cCurrentPath[FILENAME_MAX];

  if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
  {
    return errno;
  }

  cCurrentPath[sizeof(cCurrentPath)-1] = '\0'; /* not really required */

  //TODO:change to leverage the path code written by Mike
  std::string vertex_file_path = (std::string)cCurrentPath + "\\Shaders\\" + (std::string)vertexFileName;
  std::string fragment_file_path = (std::string)cCurrentPath + "\\Shaders\\" + (std::string)fragmentFileName;

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
  /*
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &VertexSourcePointer, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &FragmentSourcePointer, NULL);
  glCompileShader(fs);

  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  return shader_programme;
  */
  //glUseProgram(shader_programme);



  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling Vectex shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader

  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage(InfoLogLength);
  if (VertexShaderErrorMessage.size() > 0){
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling Fragment shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader

  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
  if (FragmentShaderErrorMessage.size() > 0){
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
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
  std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);
  fprintf(stdout, "SharderID: %d\n", (int)ProgramID);
  return ProgramID;

}



// load in and compile a vertex and fragment shader into a "program"
// again, this is taken from Cody's tutorial
GLuint _loadShader(const char* vertexFileName, const char* fragmentFileName, char*& error )
{
	error = 0;
	glewInit();
	// create shader program object and both a vertex and fragment object
	GLuint	program		= glCreateProgram();
	GLuint	vertex		= glCreateShader(GL_VERTEX_SHADER_ARB);
	GLuint fragment	    = glCreateShader(GL_FRAGMENT_SHADER_ARB);

	// read shaders into strings
	vector<string> vertexStrings;
	readShaderFile(vertexFileName,vertexStrings);
	if (vertexStrings.empty()) {
		error = "Couldn't open vertex program file!";
		return 0;
	}
	vector<string> fragmentStrings;
	readShaderFile(fragmentFileName,fragmentStrings);
	if (fragmentStrings.empty()) {
		error="Couldn't open fragment program file!";
		return 0;
	}

	// for reasons I don't understand, this program barfs if there is
	// a C++ style comment - they're easy enough to get rid of
	ridCPPcomments(vertexStrings);
	ridCPPcomments(fragmentStrings);

	// need to convert C++ strings to C strings
	vector<const char*> vertexStringsC;
	vector<const char*> fragmentStringsC;

	for(size_t i=0; i<vertexStrings.size(); i++)
		vertexStringsC.push_back(vertexStrings[i].c_str());

	for(size_t i=0; i<fragmentStrings.size(); i++)
		fragmentStringsC.push_back(fragmentStrings[i].c_str());

	// give ogl the shader source string

	// compile shaders
	glShaderSource(vertex,   (GLsizei) vertexStringsC.size(),   &(vertexStringsC[0]), NULL);
	glCompileShader(vertex);
	if (checkShaderError(vertex)) {
		error = "Could not compile vertex program";

		for(size_t i=0; i<fragmentStringsC.size(); i++)
			printf("%02d: %s\n",i,fragmentStringsC[i]);

		return 0;
	}

	glShaderSource(fragment, (GLsizei) fragmentStringsC.size(), &(fragmentStringsC[0]), NULL);
	glCompileShader(fragment);
	if (checkShaderError(fragment)) {
		error = "Could not compile fragment program";

		for(size_t i=0; i<fragmentStringsC.size(); i++)
			printf("%02d: %s\n",i,fragmentStringsC[i]);

		return 0;
	}


	// attach shaders to program object and link
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	if (checkProgramError(program)) {
		error = "Shaders failed to link!";
		return 0; // bail out, display isn't going to happen at this point.
	}

	// display results
	GLint success;
	glGetProgramiv(program, GL_OBJECT_LINK_STATUS_ARB, &success);
	if( !success ) 
	{
		error = "Shaders failed to compile and link!";
		return 0; // bail out, display isn't going to happen at this point.
	}
	else
	{
		printf("%s and %s loaded and linked successfully.\n", vertexFileName, fragmentFileName );
	}
	return program;
}
