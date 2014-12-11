#include "SurfaceOfRevolution.h"

#include "gtx\rotate_vector.hpp"

#define radiansToDegree(x) x*(180.0f/3.141592f)

glm::vec3 getNormal(glm::vec3 _center, glm::vec4 _point){

  glm::vec2 center(_center.x, _center.z);
  glm::vec2 point(_point.x, _point.z);
  double dotProduct = glm::dot(center, point);
  double rotation = (acos(dotProduct / (glm::length(point) * glm::length(point))));

  float y = 0; //= 1 / cos(rotation);

  glm::vec3 normal(point.x - center.x,y,point.y - center.y);
  return normal;
}

glm::vec2 getUV(bool first,float sample,float rotation, glm::vec4 point, float maxHeight){
  if (first){
    if ((rotation - sample) > 0){
      return glm::vec2((rotation - sample) / 360.0, point.y / maxHeight);
    }
    else{
      return glm::vec2(0 / 360.0, point.y / maxHeight);
    }

  }
  else{
      return glm::vec2(rotation / 360.0, point.y / maxHeight);
  }
  
}


void surfaceOfRevolution(glm::vec3 center, float sample, std::vector<glm::vec3>& vertexList, std::vector<glm::vec3>& normalList, std::vector<glm::vec2>& uvList){
  //make the rotation matrix;

  //convert the list into a list of glm::vec3
  std::vector<glm::vec4> oriPoints;
  std::vector<glm::vec4> lastPoints;

  float maxHeight = -1;
  float minHeight = 0;

  for (int i = 0; i < vertexList.size(); i ++){
    oriPoints.push_back(glm::vec4(vertexList[i],0));
    lastPoints.push_back(glm::vec4(vertexList[i], 0));
    if (vertexList[i].y > maxHeight){
      maxHeight = vertexList[i].y;
    }
  }

  vertexList.clear();
  float i = sample;
  for (i = sample; i < 360; i += sample){
    std::vector<glm::vec4> newPoints;
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), i, center);
    for (int j = 0; j < oriPoints.size(); j++){
      newPoints.push_back(rotMat * oriPoints[j]);
    }

    //create a triangles for them;
    for (int j = 0; j < lastPoints.size() - 1; j++){
      vertexList.push_back(glm::vec3(lastPoints[j]));
      //push the normal, the normal is just the direction from the center to the point;
      //assuming for now, its always the Y axis;
      normalList.push_back(getNormal(center, lastPoints[j]));
      //uv is just well...u is just the rotation, v is the height
      uvList.push_back(getUV(true, sample,i, lastPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j]));
      normalList.push_back(getNormal(center, newPoints[j]));
      uvList.push_back(getUV(false, sample, i, newPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(lastPoints[j+1]));
      normalList.push_back(getNormal(center, lastPoints[j + 1]));
      uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j]));
      normalList.push_back(getNormal(center, newPoints[j]));
      uvList.push_back(getUV(false, sample, i, newPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(lastPoints[j+1]));
      normalList.push_back(getNormal(center, lastPoints[j + 1]));
      uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j+1]));
      normalList.push_back(getNormal(center, newPoints[j + 1]));
      uvList.push_back(getUV(false, sample, i, newPoints[j + 1], maxHeight));
    }

    lastPoints = newPoints;
  }

  //the last point goback to origin;
  //create a triangles for them;
  for (int j = 0; j < lastPoints.size() - 1; j++){
    vertexList.push_back(glm::vec3(lastPoints[j]));
    //push the normal, the normal is just the direction from the center to the point;
    //assuming for now, its always the Y axis;
    normalList.push_back(getNormal(center, lastPoints[j]));
    //uv is just well...u is just the rotation, v is the height
    uvList.push_back(getUV(true, sample, i, lastPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j]));
    normalList.push_back(getNormal(center, oriPoints[j]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(lastPoints[j+1]));
    normalList.push_back(getNormal(center, lastPoints[j + 1]));
    uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j]));
    normalList.push_back(getNormal(center, oriPoints[j]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(lastPoints[j+1]));
    normalList.push_back(getNormal(center, lastPoints[j + 1]));
    uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j+1]));
    normalList.push_back(getNormal(center, oriPoints[j + 1]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j + 1], maxHeight));
  }
}
