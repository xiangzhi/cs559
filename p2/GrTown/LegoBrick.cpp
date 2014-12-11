#include "LegoBrick.h"

class LegoBrickHead:
	public GrObjectVBO
{
public:
	void initialize(){
		std::vector<glm::vec3> vertexList;
		std::vector<glm::vec3> normalList;
		std::vector<glm::vec2> uvList;

		vertexList.push_back(glm::vec3(2.4, 0, 0));
		vertexList.push_back(glm::vec3(2.4, 1.8, 0));
		vertexList.push_back(glm::vec3(0, 1.8, 0));

		surfaceOfRevolution(glm::vec3(0, 1, 0),20,vertexList, normalList, uvList);

		bindToArrayBuffer(vertexBuffer, vertexList);
		bindToArrayBuffer(normalBuffer, normalList);
		bindToArrayBuffer(textureBuffer, uvList);
		vertexNum = vertexList.size();
		//shaderID = loadShader("sameVertex.glsl", "sameFragment.glsl");
    //pos = glm::vec3(0, 0, 0);
    //sscale = glm::vec3(1, 1, 1);
	}

	void preDraw(DrawingState* drst){
    transform = glm::scale(scale);
    transform = glm::translate(pos) * transform;
	}

  GrObjectVBO* clone(){
    LegoBrickHead* temp = new LegoBrickHead();
    GrObjectVBO::clone(temp);
    return (GrObjectVBO*)temp;
  }
};

int LegoBrick::vertexloopList[] = { -1, -1, -1, -1, -1 };
int LegoBrick::normalloopList[] = { -1, -1, -1, -1, -1 };
int LegoBrick::uvloopList[] = { -1, -1, -1, -1, -1 };
int LegoBrick::vNumLoopList[] = { -1, -1, -1, -1, -1 };

LegoBrick::LegoBrick()
{
}

LegoBrick::LegoBrick(float _height)
{
  height = _height;
  divideTimes = 2;
}


LegoBrick::~LegoBrick()
{
}


void LegoBrick::initialize(){
	std::vector<glm::vec3> vertexList;
	std::vector<glm::vec3> normalList;
	std::vector<glm::vec2> uvList;
	//draw the bricks
	drawCube(vertexList, normalList, uvList, 31.8, 15.8, height * 9.6);
	//bind to ArrayBuffers
	bindToArrayBuffer(vertexBuffer, vertexList);
	bindToArrayBuffer(normalBuffer, normalList);
	vertexNum = vertexList.size();
	//load shaders

 
	shaderID = loadShader("legoVertex.glsl", "legoFragment.glsl");

	for (int i = 0; i < 4; i++){
		LegoBrickHead* head = new LegoBrickHead();
		head->parent = this;
		int x = 12 - (i * 8);
		head->pos = glm::vec3(x, height * 9.6, 4);
    head->scale = glm::vec3(1, 1, 1);
		head->shaderID = shaderID;
		children.push_back(head);

		head = new LegoBrickHead();
		head->parent = this;
		head->pos = glm::vec3(x, height * 9.6, -4);
    head->scale = glm::vec3(1, 1, 1);

		head->shaderID = shaderID;
		children.push_back(head);
	}
	scale = glm::vec3(6, 6, 6);
	localTransform = glm::translate(glm::vec3(0, height * 9.6 / 2, 0));
}

#include "City.h"
#include "Bob.h"

bool isOver(LegoBrick* p1, Bob * destroyer){
  float x1 = destroyer->touchPos.x + 200;
  float x2 = destroyer->touchPos.x - 200;
  float z1 = destroyer->touchPos.z + 200;
  float z2 = destroyer->touchPos.z - 200;

  if (x2 < p1->pos.x && z2 < p1->pos.z)
    if (p1->pos.x < x1 && p1->pos.z < z1)
      return true;
  return false;
}

bool sameSpace(LegoBrick* p1, LegoBrick* p2){
  float x1 = p1->pos.x + 75;
  float x2 = p1->pos.x - 75;
  float z1 = p1->pos.z + 75;
  float z2 = p1->pos.z - 75;

  if (x2 < p2->pos.x && z2 < p2->pos.z)
    if (p2->pos.x < x1 && p2->pos.z < z1)
      return true;
  return false;
}


void LegoBrick::preDraw(DrawingState* drst){

  //see what's happen here;
  Bob* destroyer = (Bob*)((City*)parent)->destroyer;
  if (drst->eatable && isOver(this, destroyer)){
    destroyed = true;
  }
  transform = glm::scale(scale);
  transform = glm::rotate(transform, ry, glm::vec3(0, 1, 0));
  transform = glm::translate(pos) * transform;
}

LegoBrick* LegoBrick::clone(){
  LegoBrick* brick = new LegoBrick();
  brick = (LegoBrick*) GrObjectVBO::clone(brick);
  //clone the children
  for (int i = 0; i < children.size(); i++){
    brick->children.push_back(((LegoBrickHead*)children[i])->clone());
  }
  return brick;
}


void LegoBrick::doSubdivision(){
  //remove all the child
  children.clear();

  if (vertexloopList[divideTimes] != -1){
    vertexBuffer = vertexloopList[divideTimes];
    normalBuffer = normalloopList[divideTimes];
    vertexNum = vNumLoopList[divideTimes];
    return;
  }

  //generate a new vertex;
  std::vector<glm::vec3> vertexList;
  std::vector<glm::vec3> normalList;
  std::vector<glm::vec2> uvList;
  //draw the bricks
  drawCube(vertexList, normalList, uvList, 20,20,20);
  //TODO:make loop subvision takes empty UV
  for (int i = 0; i < normalList.size(); i++){
    uvList.push_back(glm::vec2(0, 0));
  }

  loopSubDivision(divideTimes, vertexList, normalList, uvList);
  //rebind vertex buffer;
  bindToArrayBuffer(vertexBuffer, vertexList);
  //rebind normal buffer;
  bindToArrayBuffer(normalBuffer, normalList);
  //update the vertex Number
  vertexNum = vertexList.size();
  //save that tin the loop list
  vertexloopList[divideTimes] = vertexBuffer;
  normalloopList[divideTimes] = normalBuffer;
  vNumLoopList[divideTimes] = vertexNum;
}

#include <math.h>
#include <algorithm>

void LegoBrick::simulateUntil(unsigned long time){

  //only update 20 times per second
  //make sure some time had pass
  if (lastSimulated - time < 66.667){
    return;
  }
  lastSimulated = time;

  //get the direction
  float zDirection = -1;
  float xDirection = -1;
  float ratio = 0;
  glm::vec3 directionVector;

  //when done killing;
  if (doneDestroyed){
    LegoBrick* maxBrick = nullptr;
    float nearest = 1000000000000000;
    int maxKills = -1;
    //check whether another blob occupy similar space
    for (int i = parent->children.size() - 1; i >= 0; i--){
      LegoBrick* temp = (LegoBrick*)parent->children[i];
      if (temp == this){
        continue;
      }
      /*
      if (temp->color != color){
        continue;
      }
      */

      if (!temp->doneDestroyed){
        continue;
      }

      float distance = glm::distance(pos, temp->pos);
      if (distance < nearest){
        maxBrick = temp;
        nearest = distance;
      }

      //check whether they occupy same space
      if (sameSpace(this,temp)){
        scale.x = std::max(temp->scale.x, scale.x) + 0.5;
        scale.y = std::max(temp->scale.y, scale.y) + 0.5;
        scale.z = std::max(temp->scale.z, scale.z) + 0.5;

        //kills += temp->kills;
        //remove that bolb
        parent->children.erase(parent->children.begin() + i);
        break;
      }

      //move towards the big guy
      if (maxBrick != nullptr){
        zDirection = -1 * pos.z - maxBrick->pos.z;
        xDirection = -1 * pos.x - maxBrick->pos.x;

        glm::vec3 result = pos - maxBrick->pos;
        directionVector = result / glm::distance(pos, maxBrick->pos);
        glm::normalize(directionVector);
        ratio = abs(maxBrick->pos.z - pos.z) / (float)abs(maxBrick->pos.x - pos.x);

      }

    }


    //move and conquer others
    if (ratio > 1 && maxBrick != nullptr){
      pos.x += (rand() % 6 - 3) * directionVector.x;
      pos.z += (rand() % 12 - 9) *  directionVector.z;
    }
    else if (ratio < 1 && maxBrick != nullptr){
      pos.x += (rand() % 12 - 9) *  directionVector.x;
      pos.z += (rand() % 6 - 3) *  directionVector.z;
    }

    scale = glm::vec3(scale);
  }
 
  //slowly dissolve and drop down.
  if(destroyed && !doneDestroyed){
    doSubdivision();
    if(pos.y < -30){
      doneDestroyed = true;
    }
    pos.y -= 0.35;

    if (scale.x < 8){
      scale.x += 0.10;
    }
    if (scale.z > 8){
      scale.z += 0.10;
    }
    if (scale.y > 5){
      scale.y -= 0.30;
    }
  }
}