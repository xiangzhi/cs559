#include "Bob.h"

Bob::Bob() :GrObjectVBO("bOb the Monster")
{
  acc.y = 10;
  scale = glm::vec3(1, 1, 1);
}


Bob::~Bob()
{
}


void Bob::redoSubDivision(){

	std::vector<glm::vec3> vertexList(oriVertexList);
	std::vector<glm::vec3> normalList(oriNormalList);
	std::vector<glm::vec2> uvList(oriUvList);




	//draw the cube
	//drawCube(vertexList, normalList, uvList, 100, 100, 100);
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
  //draw the cube
  //drawCube(vertexList, normalList, uvList,400,400,400);


  oriVertexList.push_back(glm::vec3(0, 0, 400));
  oriVertexList.push_back(glm::vec3(400, 0, 0));
  oriVertexList.push_back(glm::vec3(-400, 0, 0));

  oriVertexList.push_back(glm::vec3(0, 0, 400));
  oriVertexList.push_back(glm::vec3(-400, 0, 0));
  oriVertexList.push_back(glm::vec3(0, 400, 0));

  oriVertexList.push_back(glm::vec3(0, 0, 400));
  oriVertexList.push_back(glm::vec3(400, 0, 0));
  oriVertexList.push_back(glm::vec3(0, 400, 0));

  oriVertexList.push_back(glm::vec3(-400, 0, 0));
  oriVertexList.push_back(glm::vec3(400, 0, 0));
  oriVertexList.push_back(glm::vec3(0, 400, 0));

  oriVertexList.push_back(glm::vec3(400, 0, 0));
  oriVertexList.push_back(glm::vec3(600, 600, -400));
  oriVertexList.push_back(glm::vec3(0, 400, 0));

  oriVertexList.push_back(glm::vec3(-400, 0, 0));
  oriVertexList.push_back(glm::vec3(-600, 600, -400));
  oriVertexList.push_back(glm::vec3(0, 400, 0));

  oriVertexList.push_back(glm::vec3(600, 600, -400));
  oriVertexList.push_back(glm::vec3(-600, 600, -400));
  oriVertexList.push_back(glm::vec3(0, 0, -400));

  //normals
  oriNormalList.push_back(glm::vec3(0, -1, 0));
  oriNormalList.push_back(glm::vec3(0, -1, 0));
  oriNormalList.push_back(glm::vec3(0, -1, 0));

  oriNormalList.push_back(glm::vec3(0.5, 0.5, 0));
  oriNormalList.push_back(glm::vec3(0.5, 0.5, 0));
  oriNormalList.push_back(glm::vec3(0.5, 0.5, 0));

  oriNormalList.push_back(glm::vec3(-0.5, 0.5, 0));
  oriNormalList.push_back(glm::vec3(-0.5, 0.5, 0));
  oriNormalList.push_back(glm::vec3(-0.5, 0.5, 0));

  oriNormalList.push_back(glm::vec3(0, 0, -1));
  oriNormalList.push_back(glm::vec3(0, 0, -1));
  oriNormalList.push_back(glm::vec3(0, 0, -1));

  oriNormalList.push_back(glm::vec3(0.5, 0, 0.5));
  oriNormalList.push_back(glm::vec3(0.5, 0, 0.5));
  oriNormalList.push_back(glm::vec3(0.5, 0, 0.5));

  oriNormalList.push_back(glm::vec3(-0.5, 0, 0.5));
  oriNormalList.push_back(glm::vec3(-0.5, 0, 0.5));
  oriNormalList.push_back(glm::vec3(-0.5, 0, 0.5));

  oriNormalList.push_back(glm::vec3(0, 0, -1));
  oriNormalList.push_back(glm::vec3(0, 0, -1));
  oriNormalList.push_back(glm::vec3(0, 0, -1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));

  oriUvList.push_back(glm::vec2(0, 0));
  oriUvList.push_back(glm::vec2(0, 1));
  oriUvList.push_back(glm::vec2(1, 1));


  std::vector<glm::vec3> vertexList(oriVertexList);
  std::vector<glm::vec3> normalList(oriNormalList);
  std::vector<glm::vec2> uvList(oriUvList);


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

  //to = pos;
  /*
  from.x = pos.x + 400;
  from.y = 0;
  from.z = pos.z + 400;
  */

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
      int static limit = 500;

      ax = rand() % 5;
      float xDirection = 1;

      //calculate the direction for x
      if (pos.x > limit || pos.x < -limit){
        xDirection = (pos.x / pos.x) * -1;
      }
      else if(rand() % 2 == 0){
        xDirection = -1;
      }
      
      ax *= xDirection;

      az = rand() % 5;
      float zDirection = 1;

      //calculate the direction for z
      if (pos.z > limit || pos.z < -limit){
        zDirection = (pos.z / pos.z) * -1;
      }
      else if (rand() % 2 == 0){
        zDirection = -1;
      }

      az *= zDirection;

      fall = false;
    }
  }

  //no more accelration, drop
  else if (acc.y <= 0){
    fall = true;
    return;
  }
  else if ((!physic) && pos.y > 100){
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
  if (pos.x < 0){
    from.x = pos.x + 100;
  }
  else{
    from.x = pos.x - 100;
  }
  if (pos.z < 0)
    from.z = pos.z + 100;
  else
    from.z = pos.z - 100;
  from.y = 100;

}

//write this so that the follow method would work
glm::mat4 Bob::getCamera(){

	glm::vec3 from(pos.x + 700, 1400, pos.z + 400);

	return glm::lookAt(
		from,
		pos,
		glm::vec3(0, 1, 0)
	);
}