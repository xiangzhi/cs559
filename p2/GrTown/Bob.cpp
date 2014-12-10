#include "Bob.h"
#include <Thread>

Bob::Bob() :GrObjectVBO("bOb the Monster")
{
  acc.y = 10;
  scale = glm::vec3(1, 1, 1);
}


Bob::~Bob()
{
}


void Bob::redoSubDivision(){

	std::vector<glm::vec3> vertexList;
	std::vector<glm::vec3> normalList;
	std::vector<glm::vec2> uvList;

	//draw the cube
	drawCube(vertexList, normalList, uvList, 100, 100, 100);
	//redo loopSubDivision
	loopSubDivision(divideTime, vertexList, normalList, uvList);
	//rebind vertex buffer;
	rebindToArrayBuffer(vertexBuffer, vertexList);
	//rebind normal buffer;
	rebindToArrayBuffer(normalBuffer, normalList);
	//rebindToArrayBuffer(textureBuffer, uvList);
	vertexNum = vertexList.size();
}


void Bob::initialize(){
	std::vector<glm::vec3> vertexList;
	std::vector<glm::vec3> normalList;
	std::vector<glm::vec2> uvList;

  //draw the cube
  drawCube(vertexList, normalList, uvList,400,400,400);

  loopSubDivision(divideTime, vertexList, normalList, uvList);


  bindToArrayBuffer(vertexBuffer, vertexList);
  bindToArrayBuffer(normalBuffer, normalList);
  vertexNum = vertexList.size();

  shaderID = loadShader("sameVertex.glsl", "BobFragment.glsl");
}

void Bob::preDraw(DrawingState* drst){
	
	static int last = 0;
  //check if we change the divide times
	if (last != drst->loopDivisionTimes){
		last = drst->loopDivisionTimes;
		divideTime = drst->loopDivisionTimes;
		redoSubDivision();
		//std::thread(&Bob::redoSubDivision,this).detach();
	}

  transform = glm::translate(glm::vec3(0, 200, 0));
  transform = glm::scale(scale)  * transform;
  transform = glm::translate(pos) * transform;
  //redraw = true;

  physic = drst->physics;
}

float gravity = 0.98;

void Bob::simulateUntil(unsigned long time){


  static float az = 0;
  static float ax = 0;
  static long lastTime = 0;
  
  //usg 
  static float initAcceleration = 20;
  static float maxNumOfTerms = (initAcceleration / gravity) + 1;
  static float maxHeight = (maxNumOfTerms * (initAcceleration)) / 2;

  if (time - lastTime < 33){
    return;
  }
  lastTime = time;
  //do it everyone second;

  //dropping down
  if (fall){
    if (physic){
      acc.y -= gravity;
    }
    else{
      acc.y = -10;
    }
    if (pos.y < 0){
      pos.y = 0;
      touchPos = pos;
      acc.y = initAcceleration;
      ax = rand() % 11 - 5;
      az = rand() % 11 - 5;
      fall = false;
    }
  }

  //no more accelration, drop
  else if (acc.y <= 0){
    fall = true;
    return;
  }
  else if ((!physic) && pos.y == 100){
    fall = true;
  }
  else{
    //minus gravity;
    if (physic)
      acc.y -= gravity;
    else
      acc.y = 10;
  }

  //change y;
  pos.y += acc.y;
  float change = 1.5 - (pos.y / maxHeight);
  scale = glm::vec3(change, (pos.y / maxHeight) + 0.5, change);

  //randomly jump for now
  pos.x += ax; 
  pos.z += az;

  //update from camera
  from.x = pos.x + 100;
  from.y = 100;
  from.z = pos.z + 100;

}

glm::mat4 Bob::getCamera(){

	glm::vec3 from(pos.x, pos.y + 30, pos.z + 30);

	return glm::lookAt(
		from,
		pos,
		glm::vec3(0, 1, 0)
	);
}