#include "APC.h"


APC::APC()
{
}

APC::APC(float numTimes)
{
  times = numTimes;
}


APC::~APC()
{
}


void APC::initialize(){
  // Read our .obj file
  std::vector< glm::vec3 > vertexList;
  std::vector< glm::vec2 > uvList;
  std::vector< glm::vec3 > normalList; // Won't be used at the moment.

  //the front part
  vertexList.push_back(glm::vec3(0, 0, 800));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(-400, 0, 0));

  vertexList.push_back(glm::vec3(0, 0, 800));
  vertexList.push_back(glm::vec3(-400, 0, 0));
  vertexList.push_back(glm::vec3(0, 300, 0));

  vertexList.push_back(glm::vec3(0, 0, 800));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(0, 300, 0));

  vertexList.push_back(glm::vec3(-400, 0, 0));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(0, 300, 0));


  vertexList.push_back(glm::vec3(0, 700, -400));
  vertexList.push_back(glm::vec3(0, 300,0));
  vertexList.push_back(glm::vec3(400, 0, 0));

  vertexList.push_back(glm::vec3(0, 700, -400));
  vertexList.push_back(glm::vec3(0, 300, 0));
  vertexList.push_back(glm::vec3(-400, 0, 0));

  vertexList.push_back(glm::vec3(0, 700, -400));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(-400, 0, 0));
/*
  vertexList.push_back(glm::vec3(-400, 0, 0));
  vertexList.push_back(glm::vec3(-600, 600, -400));
  vertexList.push_back(glm::vec3(0, 400, 0));

  vertexList.push_back(glm::vec3(600, 600, -400));
  vertexList.push_back(glm::vec3(-600, 600, -400));
  vertexList.push_back(glm::vec3(0, -400,0));

  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(-400, 0, 0));
  vertexList.push_back(glm::vec3(0, -400, 0));

  vertexList.push_back(glm::vec3(400, 400, -40));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(0, -400,0));

  vertexList.push_back(glm::vec3(-600, 600, -40));
  vertexList.push_back(glm::vec3(400, 0, 0));
  vertexList.push_back(glm::vec3(0, -400, 0));


  vertexList.push_back(glm::vec3(-600, 600, -40));
  vertexList.push_back(glm::vec3(600, 600, -40));
  vertexList.push_back(glm::vec3(0, 400, 0));
*/



  //normals
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));

  normalList.push_back(glm::vec3(0.5, 0.5, 0));
  normalList.push_back(glm::vec3(0.5, 0.5, 0));
  normalList.push_back(glm::vec3(0.5, 0.5, 0));

  normalList.push_back(glm::vec3(-0.5, 0.5, 0));
  normalList.push_back(glm::vec3(-0.5, 0.5, 0));
  normalList.push_back(glm::vec3(-0.5, 0.5, 0));

  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));

  normalList.push_back(glm::vec3(-0.5, 0.5, 0));
  normalList.push_back(glm::vec3(-0.5, 0.5, 0));
  normalList.push_back(glm::vec3(-0.5, 0.5, 0));

  normalList.push_back(glm::vec3(0.5, 0.5, 0));
  normalList.push_back(glm::vec3(0.5, 0.5, 0));
  normalList.push_back(glm::vec3(0.5, 0.5, 0));

  normalList.push_back(glm::vec3(0, -0.5, -0.5));
  normalList.push_back(glm::vec3(0, -0.5, -0.5));
  normalList.push_back(glm::vec3(0, -0.5, -0.5));


  //normalList.push_back(glm::vec3(0, 0, -1));
  //normalList.push_back(glm::vec3(0, 0, -1));
  //normalList.push_back(glm::vec3(0, 0, -1));

  //normalList.push_back(glm::vec3(0, -1, 0));
  //normalList.push_back(glm::vec3(0, -1, 0));
  //normalList.push_back(glm::vec3(0, -1, 0));

  //normalList.push_back(glm::vec3(0.5, 0, -0.5));
  //normalList.push_back(glm::vec3(0.5, 0, -0.5));
  //normalList.push_back(glm::vec3(0.5, 0, -0.5));

  //normalList.push_back(glm::vec3(-0.5, 0, -0.5));
  //normalList.push_back(glm::vec3(-0.5, 0, -0.5));
  //normalList.push_back(glm::vec3(-0.5, 0, -0.5));

  //normalList.push_back(glm::vec3(0, 0.25, 0.1));
  //normalList.push_back(glm::vec3(0, 0.25, 0.1));
  //normalList.push_back(glm::vec3(0, 0.25, 0.1));

  for (int i = 0; i < 7; i++){
    uvList.push_back(glm::vec2(0, 0));
    uvList.push_back(glm::vec2(0, 1));
    uvList.push_back(glm::vec2(1, 1));
  }


  loopSubDivision(times, vertexList, normalList, uvList);


  bindToArrayBuffer(vertexBuffer, vertexList);
  bindToArrayBuffer(normalBuffer, normalList);
  vertexNum = vertexList.size();

  shaderID = loadShader("LegoVertex.glsl", "LegoRedFragment.glsl");
}

void APC::preDraw(DrawingState* drst){
  transform = glm::scale(scale);
  transform = glm::rotate(transform, ry, glm::vec3(0, 1, 0));
  transform = glm::translate(pos) * transform;
}