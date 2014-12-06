#include "Loop.h"
#include <glm.hpp>
#include <vector>
#include <algorithm>

#include "Utilities\ShaderTools.H"

using std::vector;

class Point{
public:
  //the location of the Point
  glm::vec3 loc;
  //who is connected to this Point
  std::vector<void *> neighbors;
  //make sure we don't make the same edge with the partners again
  std::vector<void *> partners;
  //whether they are new or old
  bool isNew = true;
  //who to form triangle, envision to use if output to openGL
  std::vector<void *> triangleMates;
  //normal
  glm::vec3 normal;

  //who create the end
  std::vector<void *> parents;
  //list of your sons
  std::vector<void*> child;
  std::vector<void*> farChild;
  //the other end of the triangle;
  std::vector<void *> farParents;
  bool updated = false;
  glm::vec3 oldLoc;

  //methods
  Point(glm::vec3 _loc){
    loc = _loc;
  }
  void merge(Point* p){
    for (int i = 0; i < p->farParents.size(); i++){
      farParents.push_back(p->farParents[i]);
    }
    //nothing yet but soon yeah.
  }
};

class triangle{
public:
  Point * p[3];
  //12, 13, 23
  Point * np[3];
  triangle(Point* _p1, Point* _p2, Point* _p3){
    p[0] = _p1;
    p[1] = _p2;
    p[2] = _p3;
  }

};

double getB(int size){
  return 3.0 /((size + 2.0) * size);
}

void loopValueForOld(Point * p){
  double b = getB(p->neighbors.size());
  glm::vec3 neighborSum(0);

  for (int i = 0; i < p->neighbors.size(); i++){
    neighborSum += ((Point*)p->neighbors[i])->loc;
  }
  
  //multiple neighbor by B
  neighborSum *= b;


  glm::vec3 local = p->loc;
  local *= (1 - (p->neighbors.size() * b));
  local += neighborSum;
  //update local
  p->loc = local;
}

void update(Point * old, Point* nPtr){
  for (int i = 0; i < old->child.size(); i++){
    std::vector<void*> vec = ((Point*)old->child[i])->parents;
    vec.erase(std::remove(vec.begin(), vec.end(), old), vec.end());
    vec.push_back(nPtr);
    ((Point*)old->child[i])->parents = vec;
  }
  for (int i = 0; i < old->farChild.size(); i++){
    std::vector<void*> vec = ((Point*)old->farChild[i])->farParents;
    vec.erase(std::remove(vec.begin(), vec.end(), old), vec.end());
    vec.push_back(nPtr);
    ((Point*)old->farChild[i])->farParents = vec;
  }
}

void loopValueForNew(Point * p){
  ((Point*)p->parents[0])->loc;
  Point p1(((Point*)p->parents[0])->loc);
  Point p2(((Point*)p->parents[1])->loc);
  Point p3(((Point*)p->farParents[0])->loc);
  glm::vec3 p4Value(0);
  if (p->farParents.size() >= 2){
    Point p4(((Point*)p->farParents[1])->loc);
    p4Value = p4.loc;

    glm::vec3 newLoc = (3 / 8.0f) * p1.loc + (3 / 8.0f) * p2.loc +
      (1 / 8.0f) * p3.loc + (1 / 8.0f) * p4Value;
    p->loc = newLoc;

  }
  else{

    if (!p->updated){
      p->oldLoc = p->loc;
      p->updated = true;
    }
    glm::vec3 newLoc = (3 / 8.0f) * p1.loc + (3 / 8.0f) * p2.loc +
      (1 / 8.0f) * p3.loc + (1 / 8.0f) * p->oldLoc;
    p->loc = newLoc;
  }

 // return p;
}

bool isSamePoint(Point* p1, Point* p2){
  if (p1 == p2)
    return false;
  if (p1->loc.x == p2->loc.x && p1->loc.y == p2->loc.y && p1->loc.z == p2->loc.z)
    return true;
  return false;
}

bool searchList(std::vector<void*> addrList, void* ptr){
  for (int i = 0; i < addrList.size(); i++){
    if (addrList[i] == ptr)
      return true;
  }
  return false;
}


void calculate(std::vector<triangle> &triangles){
  
  std::vector<triangle> newList;
  /*
  Point* _p1 = new Point(glm::vec3(1, 0, 0));
  Point* _p2 = new Point(glm::vec3(0, 1, 0));
  Point* _p3 = new Point(glm::vec3(-1, 0, 0));
  triangles.push_back(triangle(_p1, _p2, _p3));
  Point* _p4 = new Point(glm::vec3(1, 0, 0));
  Point* _p5 = new Point(glm::vec3(-1, 0, 0));
  Point* _p6 = new Point(glm::vec3(0, -1, 0));
  triangles.push_back(triangle(_p4, _p5, _p6));
  */

  //first generate all new points along the edge;
  for (int i = 0; i < triangles.size(); i++){
    //p12
    glm::vec3 newLoc = (triangles[i].p[0]->loc + triangles[i].p[1]->loc) / 2.0f;
    Point* p = new Point(newLoc);
    p->parents.push_back(triangles[i].p[0]);
    triangles[i].p[0]->child.push_back(p);
    p->parents.push_back(triangles[i].p[1]);
    triangles[i].p[1]->child.push_back(p);
    p->farParents.push_back(triangles[i].p[2]);
    triangles[i].p[2]->farChild.push_back(p);

    triangles[i].np[0] = p;
    //p13
    newLoc = (triangles[i].p[0]->loc + triangles[i].p[2]->loc) / 2.0f;
    p = new Point(newLoc);
    p->parents.push_back(triangles[i].p[0]);
    triangles[i].p[0]->child.push_back(p);
    p->parents.push_back(triangles[i].p[2]);
    triangles[i].p[2]->child.push_back(p);
    p->farParents.push_back(triangles[i].p[1]);
    triangles[i].p[1]->farChild.push_back(p);

    triangles[i].np[1] = p;
    //p23
    newLoc = (triangles[i].p[1]->loc + triangles[i].p[2]->loc) / 2.0f;
    p = new Point(newLoc);
    p->parents.push_back(triangles[i].p[1]);
    triangles[i].p[1]->child.push_back(p);
    p->parents.push_back(triangles[i].p[2]);
    triangles[i].p[2]->child.push_back(p);
    p->farParents.push_back(triangles[i].p[0]);
    triangles[i].p[0]->farChild.push_back(p);
    triangles[i].np[2] = p;
  }

  //to store the point list
  std::vector<Point> ptrList;

  //check all points if there is an overlap, merge
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];
    for (int j = 0; j < triangles.size(); j++){
      if (i == j){
        continue;
      }
      triangle searchTri = triangles[j];
      for (int pp = 0; pp < 3; pp++){
        for (int spp = 0; spp < 3; spp++){
          if (isSamePoint(currTri.p[pp], searchTri.p[spp])){
            //combine the information of both
            //replace that point with the same reference pointer;
            currTri.p[pp]->merge(searchTri.p[spp]);
            Point* temp = searchTri.p[spp];
            update(temp, currTri.p[pp]);
            delete temp;
            //link the two points together
            searchTri.p[spp] = currTri.p[pp];
          }
        }
      }
      triangles[j] = searchTri;
    }
    triangles[i] = currTri;
  }

  //update new points neighbors table, if there is overlap, make it only one point
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];
    for (int j = 0; j < triangles.size(); j++){
      if (i == j){
        continue;
      }
      triangle searchTri = triangles[j];
      for (int pp = 0; pp < 3; pp++){
        for (int spp = 0; spp < 3; spp++){
          if (isSamePoint(currTri.np[pp], searchTri.np[spp])){
            //combine the information of both
            //replace that point with the same reference pointer;
            currTri.np[pp]->merge(searchTri.np[spp]);
            Point* temp = searchTri.np[spp];
            delete temp;

            //also use this oppertunity to calculate the new value;
            //loopValueForNew(currTri.np[pp]);
            //link the two points together
            searchTri.np[spp] = currTri.np[pp];
          }
        }
      }
      triangles[j] = searchTri;
    }
    triangles[i] = currTri;
  }

  //update all the new value, we can do it again and again for the same point, since it doesn't depend on the old value
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];
    loopValueForNew(currTri.np[0]);
    loopValueForNew(currTri.np[1]);
    loopValueForNew(currTri.np[2]);
    triangles[i] = currTri;
  }
  //here is where I was thinking of doing the border cases, but for now, we just move on...




  
  //now we go and tell every original point they have the child and populate their neighbor field.
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];
    //just add the parent to the list
    for (int j = 0; j < 3; j++){
      Point* currPoint = currTri.np[j];
      ((Point*)currPoint->parents[0])->neighbors.push_back(currPoint);
      ((Point*)currPoint->parents[1])->neighbors.push_back(currPoint);
    }
  }

  //now all the points should be link together.
  //calculate the new position for all old position
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];
    //just add the parent to the list
    for (int j = 0; j < 3; j++){
      Point* currPoint = currTri.p[j];
      loopValueForOld(currPoint);
    }
  }
  

  //now throw them all into new triangles.
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];

    //first New Triangle;
    Point* p1 = new Point(currTri.p[0]->loc);
    Point* p2 = new Point(currTri.np[0]->loc);
    Point* p3 = new Point(currTri.np[1]->loc);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.p[2]->loc);
    p2 = new Point(currTri.np[1]->loc);
    p3 = new Point(currTri.np[2]->loc);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.p[1]->loc);
    p2 = new Point(currTri.np[0]->loc);
    p3 = new Point(currTri.np[2]->loc);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.np[0]->loc);
    p2 = new Point(currTri.np[1]->loc);
    p3 = new Point(currTri.np[2]->loc);
    newList.push_back(triangle(p1, p2, p3));
    //don't delete points here, don't forget they share multiple points.
  }


  //a hacky way of knowing what I deleted.
  std::vector<void*> addrList;
  //cleaning up, remove all the stuff
  for (int i = 0; i < triangles.size(); i++){
    triangle currTri = triangles[i];

    for (int j = 0; j < 3; j++){
      if (!searchList(addrList, currTri.np[j])){
        addrList.push_back(currTri.np[j]);
        delete(currTri.np[j]);
      }
      if (!searchList(addrList, currTri.p[j])){
        delete currTri.p[j];
        addrList.push_back(currTri.p[j]);
      }
    }
  }

  triangles = newList;

}

std::vector<float> deconstruct(std::vector<triangle> triangleList){
  std::vector<float> list;
  for (int i = 0; i < triangleList.size(); i++){
    triangle t = triangleList[i];
    for (int j = 0; j < 3; j++){
      list.push_back(t.p[j]->loc.x);
      list.push_back(t.p[j]->loc.y);
      list.push_back(t.p[j]->loc.z);
    }
  }
  return list;
}

std::vector<triangle> construct(std::vector<float> floatList){
  std::vector<triangle> list;
  for (int i = 0; i < floatList.size() - 8; i += 9){
    int index = 0;

    Point* p1 = new Point(glm::vec3(floatList[i], floatList[i + 1], floatList[i + 2]));
    Point* p2 = new Point(glm::vec3(floatList[i + 3], floatList[i + 4], floatList[i + 5]));
    Point* p3 = new Point(glm::vec3(floatList[i + 6], floatList[i + 7], floatList[i + 8]));
    list.push_back(triangle(p1,p2,p3));
  }
  return list;
}

std::vector<float> loopSubDivision(std::vector<float> list){
  //construct
  std::vector<triangle> triangleList = construct(list);
  calculate(triangleList);
  return deconstruct(triangleList);
}



Loop::Loop()
{
}


Loop::~Loop()
{
}

#include "DrawingTools.h"
#include "gtc\matrix_transform.hpp"

void loopTest(int numTimes, glm::mat4 proj, glm::mat4 view, glm::mat4 model){
  static float list[] = {
  
    //0, 0, 1,
    //1, 0, 0,
    //-1, 0, 0,
    0, 0, 1,
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    -1, 0, 0,
    0, 1, 0,
    1, 0, 0,
    -1, 0, 0,
    0, 1, 0
    
    /*
    0,1,0,
    0,0,1,
    0,0,-1,
    0,0,1,
    1,0,0,
    0,0,-1,
    */
  };

  static vector<float> vlist;
  static vector<float> nvlist;

  static GLuint  vertexBuffer;
  static GLuint shaderID;
  model = model * glm::scale(glm::mat4(1.0f),glm::vec3(15, 10, 30)) 
    * glm::translate(glm::mat4(1.0f),glm::vec3(10,10,0));
  glm::mat4 MVP = proj * view * model;

  static bool first = true;

  if (first){
    first = false;
    for (int i = 0; i < (sizeof(list) / sizeof(float)); i++){
      vlist.push_back(list[i]);
    }
    nvlist = vlist;
    for (int j = 0; j < numTimes; j++){
      nvlist = loopSubDivision(nvlist);
    }
    bindToArrayBuffer(vertexBuffer,nvlist.size() * sizeof(float), &nvlist[0]);
    char* err;
    shaderID = loadShader("testVertex.glsl", "testFragment.glsl", err);
  }
  
  glm::vec3 sun(0, 1, 0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  //get the location of the values outside, since we reusing shader
  GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
  GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
  GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
  //draw the four sides;
  for (int i = 0; i < 5; i++){

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    //get location of sun
    // glUniform1i(sampleLoc, textureId);
    glUseProgram(shaderID);

    //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
    //glUniform3f(sunID, sun.x, sun.y, sun.z);

    //Send Uniform Values to shader

    //MVP = MVP * glm::translate(glm::mat4(1.0f),glm::vec3(10, 10, 0));

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //draw the box
    glDrawArrays(GL_TRIANGLES, 0, nvlist.size() / 3 );
    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
  }
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);








}