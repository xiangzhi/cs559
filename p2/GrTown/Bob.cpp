#include "Bob.h"


Bob::Bob()
{
  acc.y = 10;
  scale = glm::vec3(1, 1, 1);
}


Bob::~Bob()
{
}

void Bob::initialize(){

  std::vector<glm::vec3> vertexList;
  std::vector<glm::vec3> normalList;
  std::vector<glm::vec2> uvList;
  //draw the cube
  drawCube(vertexList, normalList, uvList,100,100,100);

  loopSubDivision(divideTime, vertexList, normalList, uvList);


  bindToArrayBuffer(vertexBuffer, vertexList);
  bindToArrayBuffer(normalBuffer, normalList);
  vertexNum = vertexList.size();

  shaderID = loadShader("testVertex.glsl", "BobFragment.glsl");
}

void Bob::preDraw(){
  transform = glm::translate(glm::vec3(0, 50, 0));
  transform = glm::scale(scale)  * transform;
  transform = glm::translate(pos) * transform;
}

float gravity = 0.98;

void Bob::simulateUntil(unsigned long time){


  static float az = 0;
  static float ax = 0;
  static bool physic = true;
  static long lastTime = 0;
  
  //usg 
  static float initAcceleration = 15;
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
      acc.y = -1;
    }
    if (pos.y < 0){
      pos.y = 0;
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
      acc.y = 1;
  }

  //change y;
  pos.y += acc.y;
  float change = 1.5 - (pos.y / maxHeight);
  scale = glm::vec3(change, (pos.y / maxHeight) + 0.5, change);

  //randomly jump for now
  pos.x += ax; 
  pos.z += az;
}