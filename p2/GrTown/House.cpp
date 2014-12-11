#include "House.h"

int House::vertexloopList[] = { -1, -1, -1, -1, -1 };
int House::normalloopList[] = { -1, -1, -1, -1, -1 };
int House::uvloopList[] = { -1, -1, -1, -1, -1 };
int House::vNumLoopList[] = { -1, -1, -1, -1, -1 };


House::House()
{

}


House::~House()
{
}

#include "City.h"
#include "Bob.h"


bool House::isOver(Bob * destroyer){
  float x1 = destroyer->touchPos.x + 200;
  float x2 = destroyer->touchPos.x - 200;
  float z1 = destroyer->touchPos.z + 200;
  float z2 = destroyer->touchPos.z - 200;

  if (x2 < pos.x && z2 < pos.z)
    if (pos.x < x1 && pos.z < z1)
      return true;
  return false;
}

bool House::sameSpace(House * destroyer){
  float x1 = destroyer->pos.x + 75;
  float x2 = destroyer->pos.x - 75;
  float z1 = destroyer->pos.z + 75;
  float z2 = destroyer->pos.z - 75;

  if (x2 < pos.x && z2 < pos.z)
  if (pos.x < x1 && pos.z < z1)
    return true;
  return false;
}


void House::redoSubDivision(){
  //loop up the table first;
  //cannot find, we going to do it once
  if (vertexloopList[divideTime] == -1){
    std::vector<glm::vec3> vertexList(*vertexList);
    std::vector<glm::vec3> normalList(*normalList);
	std::vector<glm::vec2> uvList;// (*uvList);
	for(int i = 0; i < normalList.size();i++){
		uvList.push_back(glm::vec2(0, 0));
	}

    loopSubDivision(divideTime, vertexList, normalList, uvList);
    //rebind vertex buffer;
    bindToArrayBuffer(vertexBuffer, vertexList);
    //rebind normal buffer;
    bindToArrayBuffer(normalBuffer, normalList);
    //rebindToArrayBuffer(textureBuffer, uvList);
    vertexNum = vertexList.size();


    vertexloopList[divideTime] = vertexBuffer;
    normalloopList[divideTime] = normalBuffer;
    //uvloopList[divideTime] = textureBuffer;
    vNumLoopList[divideTime] = vertexNum;
  }
  else{
    vertexBuffer = vertexloopList[divideTime];
    normalBuffer = normalloopList[divideTime];
    //textureBuffer = uvloopList[divideTime];
    vertexNum = vNumLoopList[divideTime];
  }
}

void House::preDraw(DrawingState * drst){

  static int last = 0;
  //check if we change the divide times
  if (last != drst->loopDivisionTimes){
    last = drst->loopDivisionTimes;
    divideTime = drst->loopDivisionTimes;
    //redoSubDivision();
    //std::thread(&Bob::redoSubDivision,this).detach();
  }


  transform = glm::scale(scale);
  transform = glm::translate(pos) * transform;

  //nothing yet
  if (destroyed){
    return;
  }

  //see what's happen here;
  Bob* destroyer =(Bob*) ((City*)parent)->destroyer;
  if (isOver(destroyer)){
    destroyed = true;
    //transform = glm::scale(glm::vec3(0, 0, 0));
    return;
  }


}

#include "Loop.h"

void House::simulateUntil(unsigned long time){

  if (lastSimulation - time < 66.667){
    return;
  }
  lastSimulation = time;
  float zDirection = -1;
  float xDirection = -1;
  float ratio = 0;
  glm::vec3 directionVector;
  if (doneDestroyed){
    House* max = nullptr;
    float nearest = 1000000000000000;
    int maxKills = -1;
    //check whether another blob occupy similar space
    for (int i = parent->children.size() - 1; i >= 0; i--){
      House* temp = (House*)parent->children[i];
      if (temp == this){
        continue;
      }
      if (temp->color != color){
        continue;
      }

      if (!temp->doneDestroyed){
        continue;
      }

      float distance = glm::distance(pos, temp->pos);
      if (distance < nearest){
        max = temp;
        nearest = distance;
      }

      //check whether they occupy same space
      if (sameSpace(temp)){
        scale.x += (temp->scale.x - 1);
        scale.y += (temp->scale.y - 0.8);
        scale.z += (temp->scale.z - 1);

        kills += temp->kills;
        //remove that bolb
        parent->children.erase(parent->children.begin() + i);
        break;
      }

      //move towards the big guy
      if (max != nullptr){
        zDirection = -1 * pos.z - max->pos.z;
        xDirection = -1 * pos.x - max->pos.x;

        glm::vec3 result = pos - max->pos;
        directionVector = result / glm::distance(pos, max->pos);
        glm::normalize(directionVector);
        ratio = abs(max->pos.z - pos.z) / (float ) abs(max->pos.x - pos.x);
        
      }

    }
    

    //move and conquer others
    if (ratio > 1 && max != nullptr){
      pos.x += (rand() % 6 - 3) * directionVector.x;
      pos.z += (rand() % 12 - 9) *  directionVector.z;
    }
    else if(ratio < 1 && max != nullptr){
      pos.x += (rand() % 12 - 9) *  directionVector.x;
      pos.z += (rand() % 6 - 3) *  directionVector.z;
    }

    scale = glm::vec3(scale);
  }

  //slowly dissolve and drop down.
  if (destroyed && !doneDestroyed){
    redoSubDivision();
    if (pos.y < 10){
      doneDestroyed = true;
    }
    pos.y -= 5;
    scale = glm::vec3(1.2, 0.9, 1.2);
  }
}

