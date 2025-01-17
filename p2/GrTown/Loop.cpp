// CS 559 - Project 2 - Xiang Zhi Tan
//Loop.cpp
//Do the loop algorithm for the object

#include "Loop.h"
#include <glm.hpp>
#include <vector>
#include <algorithm>

#include "Utilities\ShaderTools.H"

using std::vector;


//data structure that represent each point
class Point{
public:

  //who is connected to this Point
  std::vector<void *> neighbors;;
  //whether they are new or old
  bool isNew = true;

  /* properties of the point */
  //location
  glm::vec3 loc;
  //normal
  glm::vec3 normal;
  //color
  glm::vec3 color;
  //texture
  glm::vec2 texture;

  //copy it
  Point(Point * p){
    loc = p->loc;
    normal = p->normal;
    color = p->color;
    texture = p->texture;
  }

  ~Point(){
    //neighbors.clear();
    //parents.clear();
    //farParents.clear();
    //farChild.clear();
    //child.clear();
  }

  //who create the end
  std::vector<void *> parents;
  //list of your sons
  std::vector<void*> child;
  std::vector<void*> farChild;
  //the other end of the triangle;
  std::vector<void *> farParents;
  bool updated = false;


  glm::vec3 oldLoc;
  glm::vec3 oldNormal;
  glm::vec2 oldTexture;

  //methods
  Point(glm::vec3 _loc){
    loc = _loc;
  }
  void addNormal(float x, float y, float z){
    normal = glm::vec3(x, y, z);
  }
  void addUV(float u, float v){
    texture = glm::vec2(u, v);
  }
  void merge(Point* p){
    for (int i = 0; i < p->farParents.size(); i++){
      farParents.push_back(p->farParents[i]);
    }

    //merge the average of both values for property
    normal = (p->normal + normal) / 2.0f;
    texture = (p->texture + texture) / 2.0f;
    color = (p->color + color) / 2.0f;
  }
};

//data structure that represent each triangle
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

//get the B value for the Loop Algorthim
double getB(int size){
  return 3.0 / ((size + 2.0) * size);
}

//updae the link betwen the old point and the new point
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


//how to calculate for vec3 - normal or vertex
glm::vec3 calculateNewValue(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4){
  return (3 / 8.0f) * p1 + (3 / 8.0f) * p2 + (1 / 8.0f) * p3 + (1 / 8.0f) * p4;
}

//how to calculate for vec2 - texture
glm::vec2 calculateNewValue(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4){
  return (3 / 8.0f) * p1 + (3 / 8.0f) * p2 + (1 / 8.0f) * p3 + (1 / 8.0f) * p4;
}


//update the vertex at the point p
//based on information of the neighbor stored inside
void loopValueForOld(Point * p){

  double b = getB(p->neighbors.size());
  glm::vec3 neighborSum(0);

  //update location
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

  neighborSum = glm::vec3(0);
  for (int i = 0; i < p->neighbors.size(); i++){
    neighborSum += ((Point*)p->neighbors[i])->normal;
  }
  //multiple neighbor by B
  neighborSum *= b;
  local = p->normal;
  local *= (1 - (p->neighbors.size() * b));
  local += neighborSum;
  //update local
  p->normal = glm::normalize(local);

  //do the texture
  glm::vec2 textureSum(0);
  for (int i = 0; i < p->neighbors.size(); i++){
    textureSum += ((Point*)p->neighbors[i])->texture;
  }
  //multiple neighbor by B
  textureSum *= b;
  glm::vec2 textureLocal = p->texture;
  textureLocal *= (1 - (p->neighbors.size() * b));
  textureLocal += textureSum;
  //update local
  p->texture = textureLocal;
}

//update the new point with information from neighbor
void loopValueForNew(Point * p){
  ((Point*)p->parents[0])->loc;
  Point p1(((Point*)p->parents[0]));
  Point p2(((Point*)p->parents[1]));
  Point p3(((Point*)p->farParents[0]));
  glm::vec3 p4Value(0);
  if (p->farParents.size() >= 2){
    Point p4(((Point*)p->farParents[1]));

    p->loc = calculateNewValue(p1.loc, p2.loc, p3.loc, p4.loc);
    p->normal = glm::normalize(calculateNewValue(p1.normal, p2.normal, p3.normal, p4.normal));
    p->texture = calculateNewValue(p1.texture, p2.texture, p3.texture, p4.texture);

  }
  else{

    if (!p->updated){
      p->oldLoc = p->loc;
      p->oldTexture = p->texture;
      p->oldNormal = p->normal;
      p->updated = true;
    }
    p->loc = calculateNewValue(p1.loc, p2.loc, p3.loc, p->oldLoc);
    p->normal = glm::normalize(calculateNewValue(p1.normal, p2.normal, p3.normal, p->oldNormal));
    p->texture = calculateNewValue(p1.texture, p2.texture, p3.texture, p->oldTexture);
  }

  // return p;
}



//check whether the two point occupy the same space
bool isSamePoint(Point* p1, Point* p2){
  if (p1 == p2)
    return false;
  if (p1->loc.x == p2->loc.x && p1->loc.y == p2->loc.y && p1->loc.z == p2->loc.z)
    return true;
  return false;
}

//check whether the point is already in the list
bool searchList(std::vector<void*> addrList, void* ptr){
  for (int i = 0; i < addrList.size(); i++){
    if (addrList[i] == ptr)
      return true;
  }
  return false;
}

//combined two points, by averaging both points.
Point* getAveragePoint(Point* p1, Point* p2){
  glm::vec3 newLoc = (p1->loc + p2->loc) / 2.0f;
  Point* p = new Point(newLoc);
  //get average of color/texture/uv
  p->normal = glm::normalize((p1->normal + p2->normal) / 2.0f);
  p->texture = (p1->texture + p2->texture) / 2.0f;
  return p;
}

//do the calculation of loop algorithm for times.
void calculate(std::vector<triangle> &triangles, int times){

  for (int i = 0; i < times; i++){
    std::vector<triangle> newList;

    //first generate all new points along the edge;
    for (int i = 0; i < triangles.size(); i++){
      //p12
      Point* p = getAveragePoint(triangles[i].p[0], triangles[i].p[1]);
      p->parents.push_back(triangles[i].p[0]);
      triangles[i].p[0]->child.push_back(p);
      p->parents.push_back(triangles[i].p[1]);
      triangles[i].p[1]->child.push_back(p);
      //updat the far parents(the vertex on the triangle that is not the parents)
      p->farParents.push_back(triangles[i].p[2]);
      triangles[i].p[2]->farChild.push_back(p);

      triangles[i].np[0] = p;

      //p13
      p = getAveragePoint(triangles[i].p[0], triangles[i].p[2]);
      p->parents.push_back(triangles[i].p[0]);
      triangles[i].p[0]->child.push_back(p);
      p->parents.push_back(triangles[i].p[2]);
      triangles[i].p[2]->child.push_back(p);
      p->farParents.push_back(triangles[i].p[1]);
      triangles[i].p[1]->farChild.push_back(p);

      triangles[i].np[1] = p;
      //p23
      p = getAveragePoint(triangles[i].p[1], triangles[i].p[2]);
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
    //to fix the weird delete problem
    std::vector<void*> deleteList;
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
              searchTri.np[spp] = currTri.np[pp];

              deleteList.push_back(temp);
            }
          }
        }
        triangles[j] = searchTri;
      }
      triangles[i] = currTri;
    }

    //delete the points
    for (int j = 0; j < deleteList.size(); j++){
      delete deleteList[j];
    }

    //update all the new value, we can do it again and again for the same point, since it doesn't depend on the old value
    for (int i = 0; i < triangles.size(); i++){
      triangle currTri = triangles[i];
      loopValueForNew(currTri.np[0]);
      loopValueForNew(currTri.np[1]);
      loopValueForNew(currTri.np[2]);
      triangles[i] = currTri;
    }

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
      Point* p1 = new Point(currTri.p[0]);
      Point* p2 = new Point(currTri.np[0]);
      Point* p3 = new Point(currTri.np[1]);
      newList.push_back(triangle(p1, p2, p3));

      p1 = new Point(currTri.p[2]);
      p2 = new Point(currTri.np[1]);
      p3 = new Point(currTri.np[2]);
      newList.push_back(triangle(p1, p2, p3));

      p1 = new Point(currTri.p[1]);
      p2 = new Point(currTri.np[0]);
      p3 = new Point(currTri.np[2]);
      newList.push_back(triangle(p1, p2, p3));

      p1 = new Point(currTri.np[0]);
      p2 = new Point(currTri.np[1]);
      p3 = new Point(currTri.np[2]);
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
}

//do the calcluation for loop algorithm once
void calculate(std::vector<triangle> &triangles){

  std::vector<triangle> newList;

  //first generate all new points along the edge;
  for (int i = 0; i < triangles.size(); i++){
    //p12
    Point* p = getAveragePoint(triangles[i].p[0], triangles[i].p[1]);
    p->parents.push_back(triangles[i].p[0]);
    triangles[i].p[0]->child.push_back(p);
    p->parents.push_back(triangles[i].p[1]);
    triangles[i].p[1]->child.push_back(p);
    //updat the far parents(the vertex on the triangle that is not the parents)
    p->farParents.push_back(triangles[i].p[2]);
    triangles[i].p[2]->farChild.push_back(p);

    triangles[i].np[0] = p;

    //p13
    p = getAveragePoint(triangles[i].p[0], triangles[i].p[2]);
    p->parents.push_back(triangles[i].p[0]);
    triangles[i].p[0]->child.push_back(p);
    p->parents.push_back(triangles[i].p[2]);
    triangles[i].p[2]->child.push_back(p);
    p->farParents.push_back(triangles[i].p[1]);
    triangles[i].p[1]->farChild.push_back(p);

    triangles[i].np[1] = p;
    //p23
    p = getAveragePoint(triangles[i].p[1], triangles[i].p[2]);
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
  //to fix the weird delete problem
  std::vector<void*> deleteList;
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
            searchTri.np[spp] = currTri.np[pp];
            
            deleteList.push_back(temp);        
          }
        }
      }
      triangles[j] = searchTri;
    }
    triangles[i] = currTri;
  }

  //delete the points
  for (int j = 0; j < deleteList.size(); j++){
    delete deleteList[j];
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
    Point* p1 = new Point(currTri.p[0]);
    Point* p2 = new Point(currTri.np[0]);
    Point* p3 = new Point(currTri.np[1]);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.p[2]);
    p2 = new Point(currTri.np[1]);
    p3 = new Point(currTri.np[2]);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.p[1]);
    p2 = new Point(currTri.np[0]);
    p3 = new Point(currTri.np[2]);
    newList.push_back(triangle(p1, p2, p3));

    p1 = new Point(currTri.np[0]);
    p2 = new Point(currTri.np[1]);
    p3 = new Point(currTri.np[2]);
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

//convert the list of vertex,normal and uv into the data structure of loop from glm::vec3
void construct(std::vector<triangle>& list, std::vector<glm::vec3>& vertexList, std::vector<glm::vec3>& normallist, std::vector<glm::vec2>& uvlist){
  int uvIndex = 0;
  for (int i = 0; i < vertexList.size() - 2; i += 3){

    Point* p1 = new Point(vertexList[i]);
    p1->normal = normallist[i];
    if (uvlist.size() != 0){
      p1->texture = uvlist[i];
    }
    else{
      p1->addUV(0, 0);
    }

    Point* p2 = new Point(vertexList[i+1]);
    p2->normal = normallist[i+1];
    if (uvlist.size() != 0){
      p2->texture = uvlist[i+1];
    }
    else{
      p2->addUV(0, 0);
    }

    Point* p3 = new Point(vertexList[i + 2]);
    p3->normal = normallist[i + 2];
    if (uvlist.size() != 0){
      p3->texture = uvlist[i + 2];
    }
    else{
      p3->addUV(0, 0);
    }

    list.push_back(triangle(p1, p2, p3));
  }
}
//convert back the list of vertex,normal and uv from the data structure
void deconstruct(std::vector<triangle>& triangleList, std::vector<glm::vec3>& vertexList, std::vector<glm::vec3>& normalList, std::vector<glm::vec2>& uvList){
  std::vector<float> list;
  for (int i = 0; i < triangleList.size(); i++){
    triangle t = triangleList[i];

    for (int j = 0; j < 3; j++){
      vertexList.push_back(t.p[j]->loc);
      normalList.push_back(t.p[j]->normal);
      uvList.push_back(t.p[j]->texture);
    }
  }
}
//convert the list of vertex,normal and uv into the data structure of loop from float list
std::vector<triangle> construct(std::vector<float>& vertexList, std::vector<float>& normallist, std::vector<float>& uvlist){
  std::vector<triangle> list;
  int uvIndex = 0;
  for (int i = 0; i < vertexList.size() - 8; i += 9){

    Point* p1 = new Point(glm::vec3(vertexList[i], vertexList[i + 1], vertexList[i + 2]));
    p1->addNormal(normallist[i], normallist[i + 1], normallist[i + 2]);
    if (uvlist.size() != 0){
      p1->addUV(uvlist[uvIndex], uvlist[uvIndex + 1]);
    }
    else{
      p1->addUV(0, 0);
    }

    Point* p2 = new Point(glm::vec3(vertexList[i + 3], vertexList[i + 4], vertexList[i + 5]));
    p2->addNormal(normallist[i + 3], normallist[i + 4], normallist[i + 5]);
    if (uvlist.size() != 0){
      p2->addUV(uvlist[uvIndex + 2], uvlist[uvIndex + 3]);
    }
    else{
      p2->addUV(0, 0);
    }

    Point* p3 = new Point(glm::vec3(vertexList[i + 6], vertexList[i + 7], vertexList[i + 8]));
    p3->addNormal(normallist[i + 6], normallist[i + 7], normallist[i + 8]);
    if (uvlist.size() != 0){
      p3->addUV(uvlist[uvIndex + 4], uvlist[uvIndex + 5]);
    }
    else{
      p3->addUV(0, 0);
    }

    list.push_back(triangle(p1, p2, p3));

    uvIndex += 6;
  }
  return list;
}
//convert back the list of vertex,normal and uv from the data structure
std::vector<float> deconstruct(std::vector<triangle> triangleList, std::vector<float>& vertexList, std::vector<float>& normalList, std::vector<float>& uvList){
  std::vector<float> list;
  for (int i = 0; i < triangleList.size(); i++){
    triangle t = triangleList[i];
    for (int j = 0; j < 3; j++){
      vertexList.push_back(t.p[j]->loc.x);
      vertexList.push_back(t.p[j]->loc.y);
      vertexList.push_back(t.p[j]->loc.z);

      normalList.push_back(t.p[j]->normal.x);
      normalList.push_back(t.p[j]->normal.y);
      normalList.push_back(t.p[j]->normal.z);

      uvList.push_back(t.p[j]->texture.x);
      uvList.push_back(t.p[j]->texture.y);

    }
  }
  return list;
}

//external interface for the loop algorthims
void loopSubDivision(std::vector<float>& vertexlist, std::vector<float>& normallist, std::vector<float>& uvlist){
  //construct
  std::vector<triangle> triangleList = construct(vertexlist, normallist, uvlist);
  calculate(triangleList);
  //clear the list first
  vertexlist.clear();
  normallist.clear();
  uvlist.clear();


  deconstruct(triangleList, vertexlist, normallist, uvlist);
}
void loopSubDivision(int times, std::vector<glm::vec3>& vertexlist, std::vector<glm::vec3>& normallist, std::vector<glm::vec2>& uvlist){
  //construct
  std::vector<triangle> triangleList;
  construct(triangleList, vertexlist, normallist, uvlist);
  calculate(triangleList,times);
  //clear the list first
  vertexlist.clear();
  normallist.clear();
  uvlist.clear();
  deconstruct(triangleList, vertexlist, normallist, uvlist);
}