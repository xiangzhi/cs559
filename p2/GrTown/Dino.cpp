#include "Dino.h"
#include "gtc\matrix_transform.hpp"

#define degreesToRadians(x) x*(3.141592f/180.0f)
#define radiansToDegree(x) x*(180.0f/3.141592f)

int numTimes = 0;

class DinoLeg :
  public GrObjectVBO{
public:
  float rotation;
  void initialize(){
    //int numTimes = 1;

    std::vector<float>vertexList;
    std::vector<float>normalList;
    std::vector<float>textureList;


    draw3DRect(vertexList, normalList, 4, 4, 4, 4, 10);

    //drawCube(vertexList, normalList, 10, 10, 10);
    getTextureList(36, textureList, 100);

    //do the loop subdivision calculations
    for (int j = 0; j < numTimes; j++){
      loopSubDivision(vertexList, normalList, textureList);
    }
    //bind to the VBO
    bindToArrayBuffer(vertexBuffer, vertexList.size() * sizeof(float), &vertexList[0]);
    bindToArrayBuffer(normalBuffer, normalList.size() * sizeof(float), &normalList[0]);
    bindToArrayBuffer(textureBuffer, textureList.size() * sizeof(float), &textureList[0]);
    vertexNum = vertexList.size() / 3;
  }

  DinoLeg(std::string name) :GrObjectVBO(name.c_str()){

  }

  DinoLeg() :GrObjectVBO(){
    //lol
    rotation = 0;
  }
  bool hand = false;
  void preDraw(DrawingState* drst){
    
    glm::vec3 trans = translation;
    transform = glm::mat4(1.0f);
    if (hand){
     // transform = transform * glm::rotate(glm::mat4(1), , glm::vec3(1, 0, 0));
     // trans += glm::vec3(0, -30, 0);
    }
    transform = transform * glm::rotate(glm::mat4(1), rotation, glm::vec3(1, 0, 0));
    transform = transform * glm::translate(glm::mat4(1.0f), trans);
  }

  // note - we might not really get to the time in one shot
  // so let it take up to 10 tries
  void simulateUntil(unsigned long t)
  {
    advanceRotation();
  }
  float incr = 0.25;
  void advanceRotation(){
    rotation += incr;
    if (rotation > 45){
      incr *= -1;
      rotation = 45;
    }
    else if (rotation < -45){
      incr *= -1;
      rotation = -45;
    }
  }
};


class DinoHead : public GrObjectVBO{
  void initialize(){
    //int numTimes = 1;

    std::vector<float>vertexList;
    std::vector<float>normalList;
    std::vector<float>textureList;


    draw3DRect(vertexList, normalList, 4, 4, 4, 4, 4);

    //drawCube(vertexList, normalList, 10, 10, 10);
    getTextureList(36, textureList, 100);

    //do the loop subdivision calculations
    for (int j = 0; j < numTimes; j++){
      loopSubDivision(vertexList, normalList, textureList);
    }
    //bind to the VBO
    bindToArrayBuffer(vertexBuffer, vertexList.size() * sizeof(float), &vertexList[0]);
    bindToArrayBuffer(normalBuffer, normalList.size() * sizeof(float), &normalList[0]);
    bindToArrayBuffer(textureBuffer, textureList.size() * sizeof(float), &textureList[0]);
    vertexNum = vertexList.size() / 3;
  }
  void preDraw(DrawingState* drst){
    glm::vec3 trans = translation;
    transform = glm::mat4(1.0f);
    transform = transform * glm::translate(glm::mat4(1.0f), trans);
  }
};


Dino::Dino()
{
  transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), glm::vec3(0, 10, 0));
  GrObjectVBO *leg = new DinoLeg();
  leg->translation = glm::vec3(12, -20, 0);
  leg->parent = this;
  ((DinoLeg *)leg)->incr = 0.25;
  children.push_back(leg);

  GrObjectVBO *leg2 = new DinoLeg();
  leg2->translation = glm::vec3(-12, -20, 0);
  leg2->parent = this;
  ((DinoLeg * )leg2)->incr = -0.25;
  children.push_back(leg2);

  GrObjectVBO *hand1 = new DinoLeg();
  hand1->translation = glm::vec3(12, 20, 0);
  hand1->parent = this;
  ((DinoLeg *)hand1)->incr = -0.25;
  ((DinoLeg *)hand1)->hand = true;

  children.push_back(hand1);

  GrObjectVBO *hand2 = new DinoLeg();
  hand2->translation = glm::vec3(-12, 20, 0);
  hand2->parent = this;
  ((DinoLeg *)hand2)->incr = 0.25;
  ((DinoLeg *)hand2)->hand = true;
  children.push_back(hand2);


  GrObjectVBO *head = new DinoHead();
  head->translation = glm::vec3(0, 20, 0);
  head->parent = this;
  children.push_back(head);
}




Dino::~Dino()
{
}

void Dino::initialize(){

  //int numTimes = 0;

  std::vector<float>vertexList;
  std::vector<float>normalList;
  std::vector<float>textureList;


  draw3DRect(vertexList, normalList, 10, 10, 10, 10, 15);

  //drawCube(vertexList, normalList, 10, 10, 10);
  getTextureList(36, textureList,100);

  //do the loop subdivision calculations
  for (int j = 0; j < numTimes; j++){
    loopSubDivision(vertexList, normalList, textureList);
  }
  //bind to the VBO
  bindToArrayBuffer(vertexBuffer, vertexList.size() * sizeof(float), &vertexList[0]);
  bindToArrayBuffer(normalBuffer, normalList.size() * sizeof(float), &normalList[0]);
  bindToArrayBuffer(textureBuffer, textureList.size() * sizeof(float), &textureList[0]);
  char* err;
  shaderID = loadShader("DinoVertex.glsl", "DinoFragment.glsl", err);

  vertexNum = vertexList.size() / 3;
  t = fetchTexture("dinoTexture.jpg", true, true);
  useTexture = true;

  //localTransform = glm::rotate(glm::mat4(1.0f), 10.0f, glm::vec3(1, 0, 0));
  //localTransform *= glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), glm::vec3(0, 15, 0));
  
  //pass information to child
  for (int i = 0; i < children.size(); i++){
    children[i]->shaderID = shaderID;
    children[i]->useTexture = true;
    children[i]->t = t;
  }
}

void Dino::preDraw(DrawingState* drst){
  rotation = 45;
  transform = glm::mat4(1.0f);
  transform = transform * glm::rotate(glm::mat4(1), rotation, glm::vec3(0, 1, 0));
  transform = transform * glm::scale(glm::vec3(5, 5, 5));
  transform = transform * glm::translate(translation);
  transform = transform * glm::translate(glm::vec3(0,25,0));
}

void Dino::simulateUntil(unsigned long t){
  for (int i = 0; i < children.size(); i++){
    children[i]->simulateUntil(t);
  }

  if (lx > 500 || lx < -500){
    lx = dx * 500;
    dx *= -1;
  }

  if (lz > 500 || lz < -500){
    lz = dz * 500;
    dz *= -1;
  }


  ax = ((rand() % 100) / 100.0) + 0.25;
  az = ((rand() % 100) / 100.0) + 0.25;

  ax *= dx;
  az *= dz;

  lx += ax;
  lz += az;


  translation.x = lx;
  translation.z = lz;


  //front
  glm::vec3 front(0, 0, -1);
  double dotProduct = glm::dot(front, translation);
  rotation = radiansToDegree(acos(dotProduct / (glm::length(translation) * 1)));

  if (translation.x < 0){
    rotation *= -1;
  }
}
