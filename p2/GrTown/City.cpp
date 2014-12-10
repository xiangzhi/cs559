#include "City.h"


City::City()
{
}


City::~City()
{
}

#include "House.h"

void randomTexture(House* house){
  int swap = rand() % 5;

  switch (swap){
    case 0:
      house->t = fetchTexture("blueLegoBrick.png", true, true);
      break;
    case 1:
      house->t = fetchTexture("yellowLegoBrick.png", true, true);
      break;
    case 2:
      house->t = fetchTexture("whiteLegoBrick.png", true, true);
      break;
    case 3:
      house->t = fetchTexture("greenLegoBrick.png", true, true);
      break;
    case 4:
      house->t = fetchTexture("redLegoBrick.png", true, true);
      break;
  }
  //house->t = fetchTexture("blueLegoBrick.png", true, true);
  house->color = swap;
}

void City::initialize(){
  //create the base template for the houses and pass them to the children properties

  std::vector<glm::vec3> vertexList;
  std::vector<glm::vec3> normalList;
  std::vector<glm::vec2> uvList;
  //draw the house
  drawCube(vertexList, normalList, uvList, 100, 100, 100);

  //bind them first
  GLuint vertexBuffer;
  GLuint normalBuffer;
  GLuint uvBuffer;
  bindToArrayBuffer(vertexBuffer, vertexList);
  bindToArrayBuffer(normalBuffer, normalList);
  bindToArrayBuffer(uvBuffer, uvList);

  std::vector<glm::vec3> * ptrvertexList = new std::vector<glm::vec3>(vertexList);
  std::vector<glm::vec3> * ptrnormalList = new std::vector<glm::vec3>(normalList);
  std::vector<glm::vec2> * ptruvList = new std::vector<glm::vec2>(uvList);

  GLuint shaderID = loadShader("sameVertex.glsl", "sameFragment.glsl");

  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 10; j++){
      if (i == 5 && j == 5){
        continue;
      }
      //add children
      for (int k = 0; k < 2; k++){
        House* house = new House();
        randomTexture(house);
        house->vertexBuffer = vertexBuffer;
        house->normalBuffer = normalBuffer;
        house->textureBuffer = uvBuffer;
        house->shaderID = shaderID;
        house->vertexNum = vertexList.size();
        int x = -1650 + (i * 330);
        int z = -1650 + (j * 330) + k * 130;
        float yScale = 2 + ((rand() % 800) / 800.0);
        house->scale = glm::vec3(1, yScale, 1);
        house->pos = glm::vec3(x, yScale * 50, z);
        children.push_back(house);
        house->parent = this;
        house->useTexture = true;
        house->vertexList = ptrvertexList;
        house->normalList = ptrnormalList;
        house->uvList = ptruvList;


        house = new House();
        randomTexture(house);
        house->vertexBuffer = vertexBuffer;
        house->normalBuffer = normalBuffer;
        house->textureBuffer = uvBuffer;
        house->shaderID = shaderID;
        house->vertexNum = vertexList.size();
        x = -1650 + (i * 330) + 130;
        z = -1650 + (j * 330) + k * 130;
        yScale = 2 + ((rand() % 800) / 800.0);
        house->scale = glm::vec3(1, yScale, 1);
        house->pos = glm::vec3(x, yScale * 50, z);
        house->useTexture = true;
        children.push_back(house);
        house->parent = this;
        house->vertexList = ptrvertexList;
        house->normalList = ptrnormalList;
        house->uvList = ptruvList;
      }
    }
  }
}
void City::preDraw(DrawingState* drst){

}

void City::realDraw(){
  //overwrite the drawing here..
  //might do something later
}

void City::simulateUntil(unsigned long time){
  for (int i = 0; i < children.size(); i++){
    children[i]->simulateUntil(time);
  }
}