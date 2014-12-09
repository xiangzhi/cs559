#include "Building.h"


Building::Building()
: GrObjectVBO("building")
{
	//nothing here
}


Building::~Building()
{
}

#include "DrawingTools.h"


void Building::initialize(){
	//all the vertexs
	//height
	float h = 100;
	float points[] = {
		0.0f, 0.0f, 0.0f, //front triangle 
		100.0f, 0.0f, 0.0f,
		100.0f, h, 0.0f,//end front traingle 1
		0.0f, 0.0f, 0.0f, //front traingle 2 
		0.0f, h, 0.0f,
		100.0f, h, 0.0f, //end front triangle
		//right side
		100.0f, 0.0f, 0.0f, //right side triangle 1
		100.0f, 0.0f, -100.0f,
		100.0f, h, -100.0f, //end of right side triangle;
		100.0f, 0.0f, 0.0f, //right side triangle 2
		100.0f, h, 0.0f,
		100.0f, h, -100.0f, //end of right side triangle;
		//left side
		0.0f, 0.0f, 0.0f, //left side triangle 1
		0.0f, 0.0f, -100.0f,
		0.0f, h, -100.0f, //end of left side triangle;
		0.0f, 0.0f, 0.0f, //left side triangle 2
		0.0f, h, 0.0f,
		0.0f, h, -100.0f, //left of right side triangle;
		//back
		0.0f, 0.0f, -100.0f, //back triangle 
		100.0f, 0.0f, -100.0f,
		100.0f, h, -100.0f,//end back traingle 1
		0.0f, 0.0f, -100.0f, //back traingle 2 
		0.0f, h, -100.0f,
		100.0f, h, -100.0f, //end back triangle
		//top
		0.0f, h, 0.0f,
		100.0f, h, 0.0f,
		100.0f, h, -100.0f,
		0.0f, h, 0.0f,
		0.0f, h, -100.0f,
		100.0f, h, -100.0f
	};

	float normalPoints[] = {
		//front
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		//right
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		//left
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		//back
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		0, 0, -1,
		//top
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
	};

	float texturePoints[] = {
		//front
		0,0,
		0.25,0,
		0.25,1,
		0,0,
		0,1,
		//right side
		0.25,1,
		0.25, 0,
		0.50, 0,
		0.50, 1,
		0.25, 0,
		0.25, 1,
		0.50, 1,
		//left get the last coor
		0.75, 0,
		1, 0,
		1, 1,
		0.75, 0,
		0.75, 1,
		1, 1,
		//left
		0.50, 0,
		0.75, 0,
		0.75, 1,
		0.5, 0,
		0.5, 1,
		0.75, 1,
		//top
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
		0, 0,
	};

  bindToArrayBuffer(vertexBuffer, sizeof(points), points);
  //original code, for reference if failed.
	//glGenBuffers(1, &vertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

  bindToArrayBuffer(textureBuffer, sizeof (texturePoints), texturePoints);
	//glGenBuffers(1, &textureBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof (texturePoints), texturePoints, GL_STATIC_DRAW);

	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof (normalPoints), normalPoints, GL_STATIC_DRAW);

	t = fetchTexture("brick004.png", true, true);
	useTexture = true;

	char* err;
	shaderID = loadShader("sameVertex.glsl", "sameFragment.glsl", err);
	vertexNum = 30;

	//move the building to the location
	glm::mat4 op = glm::scale(glm::vec3(1, 3, 1));
  op = glm::rotate(op, 45.0f, glm::vec3(0, 1, 0));
	op = glm::translate(glm::vec3(0, 0, 200)) * op;

	transform = op;
}



