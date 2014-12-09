#include "DrawingTools.h"


void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<float> list){
  glGenBuffers(1, &ptrToBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, ptrToBuffer);
  glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(float), &list[0], GL_STATIC_DRAW);
}

void bindToArrayBuffer(GLuint& ptrToBuffer, double sizeOfLocation, void* ptrToLocation){
  glGenBuffers(1, &ptrToBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, ptrToBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeOfLocation, ptrToLocation, GL_STATIC_DRAW);
}

void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec3> list){
  glGenBuffers(1, &ptrToBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, ptrToBuffer);
  glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(glm::vec3), &list[0], GL_STATIC_DRAW);
}

void bindToArrayBuffer(GLuint& ptrToBuffer, std::vector<glm::vec2> list){
  glGenBuffers(1, &ptrToBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, ptrToBuffer);
  glBufferData(GL_ARRAY_BUFFER, list.size() * sizeof(glm::vec2), &list[0], GL_STATIC_DRAW);
}

void drawCube(std::vector<glm::vec3> &vertexList, std::vector<glm::vec3> &normalList, std::vector<glm::vec2> &uvList, float _l, float _w, float _h){

  float l = _l / 2;
  float w = _w / 2;
  float h = _h / 2;
  //front
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(l, -h, w));
  vertexList.push_back(glm::vec3(l, h, w));
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(-l, h, w));
  vertexList.push_back(glm::vec3(l, h, w));
  //right side
  vertexList.push_back(glm::vec3(l, -h, w));
  vertexList.push_back(glm::vec3(l, -h, -w));
  vertexList.push_back(glm::vec3(l, h, -w));
  vertexList.push_back(glm::vec3(l, -h, w));
  vertexList.push_back(glm::vec3(l, h, w));
  vertexList.push_back(glm::vec3(l, h, -w));
  //left side
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(-l, -h, -w));
  vertexList.push_back(glm::vec3(-l, h, -w));
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(-l, h, w));
  vertexList.push_back(glm::vec3(-l, h, -w));
  //back
  vertexList.push_back(glm::vec3(-l, -h, -w));
  vertexList.push_back(glm::vec3(l, -h, -w));
  vertexList.push_back(glm::vec3(l, h, -w));
  vertexList.push_back(glm::vec3(-l, -h, -w));
  vertexList.push_back(glm::vec3(-l, h, -w));
  vertexList.push_back(glm::vec3(l, h, -w));
  //top
  vertexList.push_back(glm::vec3(-l, h, w));
  vertexList.push_back(glm::vec3(l, h, w));
  vertexList.push_back(glm::vec3(l, h, -w));
  vertexList.push_back(glm::vec3(-l, h, w));
  vertexList.push_back(glm::vec3(-l, h, -w));
  vertexList.push_back(glm::vec3(l, h, -w));
  //bottom
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(l, -h, w));
  vertexList.push_back(glm::vec3(l, -h, -w));
  vertexList.push_back(glm::vec3(-l, -h, w));
  vertexList.push_back(glm::vec3(-l, -h, -w));
  vertexList.push_back(glm::vec3(l, -h, -w));

  //normals
  normalList.push_back(glm::vec3(0, 0, 1));
  normalList.push_back(glm::vec3(0, 0, 1));
  normalList.push_back(glm::vec3(0, 0, 1));
  normalList.push_back(glm::vec3(0, 0, 1));
  normalList.push_back(glm::vec3(0, 0, 1));
  normalList.push_back(glm::vec3(0, 0, 1));
  //right
  normalList.push_back(glm::vec3(1, 0, 0));
  normalList.push_back(glm::vec3(1, 0, 0));
  normalList.push_back(glm::vec3(1, 0, 0));
  normalList.push_back(glm::vec3(1, 0, 0));
  normalList.push_back(glm::vec3(1, 0, 0));
  normalList.push_back(glm::vec3(1, 0, 0));
  //left
  normalList.push_back(glm::vec3(-1, 0, 0));
  normalList.push_back(glm::vec3(-1, 0, 0));
  normalList.push_back(glm::vec3(-1, 0, 0));
  normalList.push_back(glm::vec3(-1, 0, 0));
  normalList.push_back(glm::vec3(-1, 0, 0));
  normalList.push_back(glm::vec3(-1, 0, 0));
  //back
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  normalList.push_back(glm::vec3(0, 0, -1));
  //top
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  //bottom
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));
  normalList.push_back(glm::vec3(0, -1, 0));

  //uv List
  for (int i = 0; i < 4; i++){
    float temp = i * 0.25;
    uvList.push_back(glm::vec2(0 + temp, 0));
    uvList.push_back(glm::vec2(0.25 +  temp, 0));
    uvList.push_back(glm::vec2(0.25 + temp, 0.25));
    uvList.push_back(glm::vec2(0 + temp, 0));
    uvList.push_back(glm::vec2(0.0 + temp, 0.25));
    uvList.push_back(glm::vec2(0.25 + temp, 0.25));
  }

  uvList.push_back(glm::vec2(0, 0.25));
  uvList.push_back(glm::vec2(0.25, 0.25));
  uvList.push_back(glm::vec2(0.25, 0.50));
  uvList.push_back(glm::vec2(0, 0.25));
  uvList.push_back(glm::vec2(0, 0.50));
  uvList.push_back(glm::vec2(0.25, 0.50));

  uvList.push_back(glm::vec2(0.25, 0.25));
  uvList.push_back(glm::vec2(0.50, 0.25));
  uvList.push_back(glm::vec2(0.5, 0.50));
  uvList.push_back(glm::vec2(0.25, 0.25));
  uvList.push_back(glm::vec2(0.25, 0.50));
  uvList.push_back(glm::vec2(0.5, 0.50));

}

void drawCube(std::vector<float> &vertexList, std::vector<float> &normalList, float l, float w, float h){
  float points[] = {
    -l, 0.0f, w, //front triangle 
    l, 0.0f, w,
    l, h, w,//end front traingle 1
    -l, 0.0f, w, //front traingle 2 
    -l, h, w,
    l, h, w, //end front triangle
    //right side
    l, 0.0f, w, //right side triangle 1
    l, 0.0f, -w,
    l, h, -w, //end of right side triangle;
    l, 0.0f, w, //right side triangle 2
    l, h, w,
    l, h, -w, //end of right side triangle;
    //left side
    -l, 0.0f, w, //left side triangle 1
    -l, 0.0f, -w,
    -l, h, -w, //end of left side triangle;
    -l, 0.0f, w, //left side triangle 2
    -l, h, w,
    -l, h, -w, //left of right side triangle;
    //back
    -l, 0.0f, -w, //back triangle 
    l, 0.0f, -w,
    l, h, -w,//end back traingle 1
    -l, 0.0f, -w, //back traingle 2 
    -l, h, -w,
    l, h, -w, //end back triangle
    //top
    -l, h, w,
    l, h, w,
    l, h, -w,
    -l, h, w,
    -l, h, -w,
    l, h, -w,
    //bottom
    - l, 0, w,
    l, 0, w,
    l, 0, -w,
    -l, 0, w,
    -l, 0, -w,
    l, 0, -w
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
    //bottom
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
  };

  for (int i = 0; i < sizeof(points)/sizeof(float); i++){
    vertexList.push_back(points[i]);
    normalList.push_back(normalPoints[i]);
  }

}

#include "gtc\matrix_transform.hpp"

void listToVector(float * list, int size, std::vector<float>& vList){
  for (int i = 0; i < size; i++){
    vList.push_back(list[i]);
  }
}

void listToVector(float * list, int size, std::vector<glm::vec3>& vList){
  for (int i = 0; i < size - 2; i += 3){
    vList.push_back(glm::vec3(list[i], list[i] + 1, list[i] + 2));
  }
}

void listToVector(float * list, int size, std::vector<glm::vec2>& vList){
  for (int i = 0; i < size - 1; i += 2){
    vList.push_back(glm::vec2(list[i], list[i] + 1));
  }
}



void addToList(std::vector<float> &list, glm::vec3 vec){
  for (int i = 0; i < 6; i++){
    list.push_back(vec.x);
    list.push_back(vec.y);
    list.push_back(vec.z);
  }
}

void draw3DRect(std::vector<float> &vertexList, std::vector<float> &normalList, float l1  , float l2 , float w1 , float w2 , float h ){
  float points[] = {
    -l2, -h, w2, //front triangle 
    l2, -h, w2,
    l1, h, w1,//end front traingle 1
    -l2, -h, w2, //front traingle 2 
    -l1, h, w1,
    l1, h, w1, //end front triangle
    //right side
    l2, -h, w2, //right side triangle 1
    l2, -h, -w2,
    l1, h, -w1, //end of right side triangle;
    l2, -h, w2, //right side triangle 2
    l1, h, w1,
    l1, h, -w1, //end of right side triangle;
    //left side
    -l2, -h, w2, //left side triangle 1
    -l2, -h, -w2,
    -l1, h, -w1, //end of left side triangle;
    -l2, -h, w2, //left side triangle 2
    -l1, h, w1,
    -l1, h, -w1, //left of right side triangle;
    //back
    -l2, -h, -w2, //back triangle 
    l2, -h, -w2,
    l1, h, -w1,//end back traingle 1
    -l2, -h, -w2, //back traingle 2 
    -l1, h, -w1,
    l1, h, -w1, //end back triangle
    //top
    -l1, h, w1,
    l1, h, w1,
    l1, h, -w1,
    -l1, h, w1,
    -l1, h, -w1,
    l1, h, -w1,
    //bottom
    -l2, -h, w2,
    l2, -h, w2,
    l2, -h, -w2,
    -l2, -h, w2,
    -l2, -h, -w2,
    l2, -h, -w2
  };

  for (int i = 0; i < sizeof(points) / sizeof(float); i++){
    vertexList.push_back(points[i]);
  }

  //calculate the normals
  glm::vec2 line = glm::vec2(sqrt(pow(w2 - w1, 2)), sqrt(pow(0 - h, 2)));
  glm::mat2 rotationMatrix;
  rotationMatrix[0][0] = 0;
  rotationMatrix[0][1] = 1;
  rotationMatrix[1][0] = 1;
  rotationMatrix[1][1] = 0;
  if (w1 > w2){
    line = line * rotationMatrix;
  }
  else if (w2 > w1){
    rotationMatrix[0][1] = -1;
    line = line * rotationMatrix;
  }
  else{
    line = glm::vec2(1, 0);
  }
  //front normal
  glm::vec3 frontNormal = glm::normalize(glm::vec3(0, line.y, -line.x));
  glm::vec3 backNormal = glm::normalize(glm::vec3(0, line.y, line.x));

  line = glm::vec2(sqrt(pow(l2 - l1, 2)), sqrt(pow(0 - h, 2)));
  if (l1 > l2){
    rotationMatrix[0][1] = 1;
    line = line * rotationMatrix;
  }
  else if (l2 > l1){
    rotationMatrix[0][1] = -1;
    line = line * rotationMatrix;
  }
  else{
    line = glm::vec2(1, 0);
  }
  
  glm::vec3 leftNormal = glm::normalize(glm::vec3(line.x, line.y, 0));
  glm::vec3 rightNormal = glm::normalize(glm::vec3(-line.x, line.y, 0));

  //add them to list
  addToList(normalList, frontNormal);
  addToList(normalList, rightNormal);
  addToList(normalList, leftNormal);
  addToList(normalList, backNormal);
  addToList(normalList, glm::vec3(0,1,0));
  addToList(normalList, glm::vec3(0, -1, 0));
}



void getTextureList(int size, std::vector<float> &uvList, int max){
  if (size % 3 != 0){
    throw std::runtime_error("gettexureList did not pass a multiple of 3(triangles)");
  }

  for (int i = 0; i < size; i += 3){
    uvList.push_back(0);
    uvList.push_back(0);
    uvList.push_back(max);
    uvList.push_back(0);
    uvList.push_back(max);
    uvList.push_back(max);
  }
}