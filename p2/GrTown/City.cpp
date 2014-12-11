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

#include "ObjectLoader.h"
#include "SurfaceOfRevolution.h"
#include "LegoBrick.h"
#include <random>



const int range = 150;
bool sameSpace(glm::vec3 pos, glm::vec3 tmp){
  float x1 = pos.x + range;
  float x2 = pos.x - range;
  float z1 = pos.z + range;
  float z2 = pos.z - range;

  if (x2 < tmp.x && z2 < tmp.z)
    if (tmp.x < x1 && tmp.z < z1)
      return true;
  return false;
}

void City::initialize(){
  //create the base template for the houses and pass them to the children properties

  //TODO::change to an arraylist of precompute if sharing did become a problem
  LegoBrick* brick = new LegoBrick(1);
  brick->initialize();
  brick->redraw = false;
  //distribution of lego pieces;
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0, 800);

  //set different shaders
  GLuint shaders[3];
  shaders[0] = loadShader("legoVertex.glsl", "legoRedFragment.glsl");
  shaders[1] = loadShader("legoVertex.glsl", "legoBlueFragment.glsl");
  shaders[2] = loadShader("legoVertex.glsl", "legoGreenFragment.glsl");


  int numBrick = 75;
  for (int i = 0; i < numBrick; i++){
    LegoBrick* brick2 = brick->clone();
    int x = floor(distribution(generator));
    int z = floor(distribution(generator));
    brick2->pos = (glm::vec3(x, 0, z));
    brick2->shaderID = shaders[rand() % 3];
    //50-50 chance of it facing another direction
    if (rand() % 2 == 0){
      brick2->ry = 90;
    }

    //check whether the current location already have a brick
    //if there is already a brick, change the height of that brick;
    bool same = false;
    int index = -1;
    for (int j = children.size() - 1; j >= 0; j--){
      glm::vec3 childLoc = children[j]->pos;
      if (sameSpace(childLoc, brick2->pos)){
        //remove for now
        same = true;
        index = j;
        break;
      }
    }

    //if overlap remove for now
    if (same){
      LegoBrick* sameBrick = (LegoBrick*)children[index];
      sameBrick->scale.y = sameBrick->scale.y * 1.4;
      //float scaleFactor = ((sameBrick->scale.y - 6) * 2);
      for (int k = 0; k < sameBrick->children.size(); k++){
        ((LegoBrick*)children[index])->children[k]->scale.y = (sameBrick->children[k]->scale.y)/1.4;
      }

      delete brick2;
      continue;
    }

    //make the children also use the same shader
    for (int k = 0; k < brick2->children.size(); k++){
      brick2->children[k]->shaderID = brick2->shaderID;
    }


    children.push_back(brick2);
    brick2->parent = this;
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