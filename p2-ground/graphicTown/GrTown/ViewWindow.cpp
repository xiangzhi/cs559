#include "ViewWindow.h"


#include <windows.h>
#include <GL\glew.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <vector>
#include "glm.hpp"
#include <time.h>
#include <gtc\matrix_transform.hpp>

#include "ShaderTools.h"


int buttonDown;
float pitch = 0;
float direction = 0;
glm::vec3 camPos = glm::vec3(0, 3, 3);


glm::mat4 getViewMatrix(){
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		camPos, // Camera is at (4,3,3), in World Space
		glm::vec3(direction, pitch, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	return View;
}


void forward(float v){

	glm::mat4 vm = getViewMatrix();

	camPos.x-= v*vm[0][2];
	camPos.y -= v*vm[1][2];
	camPos.z -= v*vm[2][2];
}


unsigned long lastUItime = 0;

//temp for idle function
void IdleFunction(void* v){
	ViewWindow* vw = (ViewWindow*)v;

	unsigned long uitime = clock();
	unsigned long te = uitime - lastUItime;
	lastUItime = uitime;


	float u = ((float)te) / 30.f;

	std::cout << "idle" << std::endl;
	if (buttonDown > 1){
		forward(u * 1);
	}

	vw->damage(1);
}


ViewWindow::ViewWindow(int x, int y, int w, int h,
	const char* l)
	: Fl_Gl_Window(x, y, w, h, l)
{
	mode(FL_RGB | FL_ALPHA | FL_DEPTH | FL_DOUBLE);
	Fl::add_idle(IdleFunction, this);
}


ViewWindow::~ViewWindow()
{
}

void InitializeGL()
{
	glClearColor(.1f, .1f, .1f, 1);
	glEnable(GL_DEPTH_TEST);
	glewInit();

	


}

class drawObj{
public:
	GLuint vbo;
	GLuint shaderID;
	drawObj(GLuint _vbo, GLuint _id){
		vbo = _vbo;
		shaderID = _id;
	}
};


std::vector<drawObj> objectsToDraw;

void initializeObject(){
	static const float g_vertex_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	
	//get a shader
	GLuint id = LoadShaders("sampleVertex.glsl", "sampleFragment.glsl");

	objectsToDraw.push_back(drawObj(ptrTovertexBuffer,id));
	/*

	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);

	*/

}




//deal with events happening in the OpenGL Window
//stole from the sample code from Prof. Gleicher, THANKS
int ViewWindow::handle(int event){
	std::cout << event << std::endl;

	static int stX = 0, stY = 0;
	static int lX = 0, lY = 0;

	switch (event) {
	case FL_PUSH:
		lX = stX = Fl::event_x();
		lY = stY = Fl::event_y();
		buttonDown = Fl::event_button();
		return 1;
	case FL_RELEASE:
		buttonDown = 0;
		return 1;
	case FL_FOCUS:
		return 1;
	case FL_KEYBOARD:
		switch (Fl::event_key()) {
		case 'z':
			break;
		case 'w':
			forward(0.5);
			damage(1);
			break;
		};
		return 1;
	case FL_DRAG:
		int mX = Fl::event_x();
		int mY = Fl::event_y();
		if (buttonDown) {
			switch (Fl::event_button()) {
			case 1:
			case 3:
				float dx = static_cast<float>(mX - lX);
				float dy = static_cast<float>(mY - lY);
				if (fabs(dx) > fabs(dy)) {
					direction -= 0.01f * dx;
				}
				else {
					pitch += 0.01f * dy;
				}
				damage(1);
				break;
			}
		}

		buttonDown = Fl::event_button();

		lX = mX; lY = mY;
		return 1;
	};




	return Fl_Gl_Window::handle(event);
}




void ViewWindow::draw()
{
	static bool firstTime = true;
	if (firstTime)
	{
		InitializeGL();
		firstTime = false;
		initializeObject();
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnableVertexAttribArray(0);
	for (int i = 0; i < objectsToDraw.size(); i++){

		GLuint vbo = objectsToDraw[i].vbo;
		GLuint shaderId = objectsToDraw[i].shaderID;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		
		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = getViewMatrix();
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

		//REMEMBER TO CALL BEFORE PASSING THE VALUES
		glUseProgram(shaderId);

		// Get a handle for our "MVP" uniform.
		// Only at initialisation time.
		GLuint MatrixID = glGetUniformLocation(shaderId, "MVP");
		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		// For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		

		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);
	}

	glDisableVertexAttribArray(0);

	/*
	// draw something
	float points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof (float), points, GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	const char* vertex_shader =
		"#version 400\n"
		"in vec3 vp;"
		"void main () {"
		"  gl_Position = vec4 (vp, 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"out vec4 frag_colour;"
		"void main () {"
		"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shader_programme);
	glBindVertexArray(vao);
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays(GL_TRIANGLES, 0, 3);
	*/
}