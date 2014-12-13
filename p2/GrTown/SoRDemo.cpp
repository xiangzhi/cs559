#include "SoRDemo.h"
#include "SurfaceOfRevolution.h"

SoRDemo::SoRDemo()
{

}

SoRDemo::SoRDemo(float _rotation)
{
  rotation = _rotation;
}



SoRDemo::~SoRDemo()
{
}


void SoRDemo::initialize(){
  std::vector<glm::vec3> vertexList;
  //add the points;
  vertexList.push_back(glm::vec3(0, 0, 0));
  vertexList.push_back(glm::vec3(2, 2.5, 0));
  vertexList.push_back(glm::vec3(8, 7.5, 0));
  vertexList.push_back(glm::vec3(10, 10, 0));
  vertexList.push_back(glm::vec3(8, 12.5, 0));
  vertexList.push_back(glm::vec3(2, 17.5, 0));
  vertexList.push_back(glm::vec3(0, 20, 0));
  vertexList.push_back(glm::vec3(2, 22.5, 0));
  vertexList.push_back(glm::vec3(8, 27.5, 0));
  vertexList.push_back(glm::vec3(10, 30, 0));
  vertexList.push_back(glm::vec3(8, 32.5, 0));
  vertexList.push_back(glm::vec3(2, 37.5, 0));
  vertexList.push_back(glm::vec3(0, 40, 0));
  std::vector<glm::vec3> normalList;
  std::vector<glm::vec2> uvList;

  surfaceOfRevolution(glm::vec3(0, 1, 0), rotation, vertexList, normalList, uvList);
  //drawCube(vertexList, normalList, uvList);

  bindToArrayBuffer(vertexBuffer, vertexList);
  bindToArrayBuffer(normalBuffer, normalList);
  bindToArrayBuffer(textureBuffer, uvList);
  vertexNum = vertexList.size();
  shaderID = loadShader("legoVertex.glsl", "legoBlueFragment.glsl");
}

void SoRDemo::preDraw(DrawingState* drst){
  transform = glm::scale(scale);
  transform = glm::rotate(transform, ry, glm::vec3(0, 1, 0));
  transform = glm::translate(pos) * transform;
}