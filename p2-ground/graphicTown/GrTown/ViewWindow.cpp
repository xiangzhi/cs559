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
#include "TextureLoader.h"


int buttonDown;
float pitch = 0;
float direction = 0;
glm::vec3 camPos = glm::vec3(0, 3, 5);


glm::mat4 getViewMatrix(){
	// Camera matrix

	//std::cout << direction << " " << pitch << std::endl;

	//get the correct direction
	glm::mat4 tmp1, tmp2, tmp3;
	/*
	tmp1 = glm::translate(tmp1, -camPos);
	tmp3 = glm::rotate(tmp2, -direction, glm::vec3(0, 1, 0)) * tmp1;
	tmp3 = glm::rotate(tmp1, -pitch, glm::vec3(1, 0, 0)) * tmp3;
	return tmp3;
	
	transMatrix(tmp1, -posX, -posY, -posZ);
	rotMatrix(tmp2, 'Y', -direction);
	multMatrix(tmp1, tmp2, tmp3);
	rotMatrix(tmp1, 'X', -pitch);
	multMatrix(tmp3, tmp1, camera);

	*/


	glm::vec3 ori(0,1,0);

	glm::mat4 View = glm::lookAt(
		camPos, // Camera is at (4,3,3), in World Space
		ori, // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	View = glm::rotate(View, -direction, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -pitch, glm::vec3(1, 0, 0));

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

	//std::cout << "idle" << std::endl;
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
	GLuint colorBuffer;
	GLuint textureBuffer;
	GLuint textureId;
	bool color;
	bool texture;
	int vertexNum;
	glm::mat4 modelMatrix;
	drawObj(GLuint _vbo, GLuint _id, int _num){
		vbo = _vbo;
		shaderID = _id;
		vertexNum = _num;
		color = false;
		texture = false;
	}
	void addColorBuffer(GLuint buffer){
		colorBuffer = buffer;
		color = true;
	}
	void addTextureBuffer(GLuint buffer){
		textureBuffer = buffer;
		texture = true;
	}
};


std::vector<drawObj> objectsToDraw;

void drawCube(){
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	//get a shader
	GLuint id = LoadShaders("sampleColorVertex.glsl", "sampleColorFragment.glsl");

	auto obj = drawObj(ptrTovertexBuffer, id, 12 * 3);
	obj.addColorBuffer(colorbuffer);

	glm::mat4 mat;
	mat = glm::rotate(mat, 45.0f, glm::vec3(0, 1, 0));
	mat = glm::translate(mat, glm::vec3(4, 0, 0)) * mat;
	obj.modelMatrix = mat;

	

	objectsToDraw.push_back(obj);

}

GLuint drawBasicCube(){
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return ptrTovertexBuffer;
}

void drawCubeT(){
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
	static const GLfloat g_uv_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};

	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	GLuint texturebuffer;
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
	

	//get a shader
	GLuint id = LoadShaders("sampleVertex.glsl", "sampleFragment.glsl");

	auto obj = drawObj(ptrTovertexBuffer, id, 12 * 3);
	obj.addTextureBuffer(texturebuffer);

	glm::mat4 mat;
	mat = glm::rotate(mat, 45.0f, glm::vec3(0, 1, 0));
	mat = glm::translate(mat, glm::vec3(4, 0, 0)) * mat;
	obj.modelMatrix = mat;


	obj.textureId = loadBMP("sampleTexture.bmp");

	objectsToDraw.push_back(obj);

}


void drawGround(){
	
	/*
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		
		-100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, 100.0f,
		
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

	GLuint shaderId = LoadShaders("simpleVertex.glsl", "simpleFragment.glsl");

	auto obj = drawObj(ptrTovertexBuffer, shaderId, 3);
	objectsToDraw.push_back(obj);
	*/
	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const float g_vertex_buffer_data[] = {
		-100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, 100.0f,
		100.0f, 0.0f, -100.0f,
		-100.0f, 0.0f, 100.0f,
	};

	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
	};



	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


	//get a shader
	GLuint id = LoadShaders("simpleVertex.glsl", "simpleFragment.glsl");
	auto obj = drawObj(ptrTovertexBuffer, id, 6);
	obj.modelMatrix = glm::mat4(1.0f);
	obj.addColorBuffer(colorbuffer);
	objectsToDraw.push_back(obj);
}



void initializeObject(){
	static const float g_vertex_buffer_data[] = {
		1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	static const GLfloat g_color_buffer_data[] = {
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
	};

	//address to vertex buffer
	GLuint ptrTovertexBuffer;
	//generate one buffer and store the address
	glGenVertexArrays(1, &ptrTovertexBuffer);
	//define what kind of buffer it is... i guess that's what it mean
	glBindBuffer(GL_ARRAY_BUFFER, ptrTovertexBuffer);
	//give vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	
	//get a shader
	GLuint id = LoadShaders("sampleColorVertex.glsl", "sampleColorFragment.glsl");
	auto obj = drawObj(ptrTovertexBuffer, id, 3);
	obj.modelMatrix = glm::mat4(1.0f);
	obj.addColorBuffer(colorbuffer);
	objectsToDraw.push_back(obj);
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
	//std::cout << event << std::endl;

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
					direction -= 0.05f * dx;
				}
				else {
					pitch += 0.05f * dy;
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

		// Cull triangles which normal is not towards the camera
		//glEnable(GL_CULL_FACE);


		firstTime = false;
		//initializeObject();
		drawCubeT();
		drawGround();
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (int i = 0; i < objectsToDraw.size(); i++){

		GLuint vbo = objectsToDraw[i].vbo;
		GLuint shaderId = objectsToDraw[i].shaderID;
		
		int vertexNum = objectsToDraw[i].vertexNum;
		glm::mat4 model = objectsToDraw[i].modelMatrix;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		if (objectsToDraw[i].color){
			GLuint colorBuffer = objectsToDraw[i].colorBuffer;
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
				);
		}

		if (objectsToDraw[i].texture){
			GLuint colorBuffer = objectsToDraw[i].textureBuffer;
			GLuint textureID = objectsToDraw[i].textureId;
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glVertexAttribPointer(
				2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
				);
				
			//get
			GLuint sampleLoc = glGetUniformLocation(shaderId, "textureSample");
			glUniform1i(sampleLoc, textureID);
		}


		// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
		// Camera matrix
		glm::mat4 View = getViewMatrix();
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = model;  // Changes for each model !
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
		

		
		glDrawArrays(GL_TRIANGLES, 0, vertexNum);
		glUseProgram(0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

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