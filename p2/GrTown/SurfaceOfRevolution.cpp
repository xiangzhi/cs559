#include "SurfaceOfRevolution.h"

#include "gtx\rotate_vector.hpp"

#define radiansToDegree(x) x*(180.0f/3.141592f)


glm::vec3 getNormal(glm::vec3 currPoint, glm::vec3 lastPoints){
  glm::vec3 surface = currPoint - lastPoints;
  glm::vec2 surfaceVec2 = glm::normalize(glm::vec2(surface));
  glm::mat2 rotMat;
  rotMat[0][0] = 0;
  rotMat[0][1] = -1;
  rotMat[1][0] = 1;
  rotMat[1][1] = 0;
  surfaceVec2 = rotMat * surfaceVec2;
  return glm::vec3(surfaceVec2.x, surfaceVec2.y, 0);
}

glm::vec3 getNormal(float rotation, glm::vec3 normal){
  return glm::rotateY(glm::vec3(normal), rotation);
}

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
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> lastNormals;



  float maxHeight = -1;
  float minHeight = 0;

  for (int i = 0; i < vertexList.size(); i ++){
    oriPoints.push_back(glm::vec4(vertexList[i],0));
    lastPoints.push_back(glm::vec4(vertexList[i], 0));
    if (vertexList[i].y > maxHeight){
      maxHeight = vertexList[i].y;
    }
  }

  //calculate the normals for each points
  for (int i = 0; i < vertexList.size() - 1; i++){
    glm::vec3 curNormal = getNormal(vertexList[i + 1], vertexList[i]);
    if (vertexList[i].x == 0){
      curNormal = glm::vec3(0, 1, 0);
    }
    if (normals.size() != 0){
      curNormal = (curNormal + normals[normals.size() - 1])/2.0f;
    }
    normals.push_back(curNormal);
    lastNormals.push_back(curNormal);
  }
  //last points normal

  if (vertexList[vertexList.size() - 1].x == 0){
    normals.push_back(glm::vec3(0, 1, 0));
    lastNormals.push_back(glm::vec3(0, 1, 0));
  }
  else{
    normals.push_back(normals[normals.size() - 1]);
    lastNormals.push_back(normals[normals.size() - 1]);
  }

  vertexList.clear();
  float i = sample;
  for (i = sample; i < 360; i += sample){
    std::vector<glm::vec4> newPoints;
    std::vector<glm::vec3> newNormals;
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), i, center);
    for (int j = 0; j < oriPoints.size(); j++){
      newPoints.push_back(rotMat * oriPoints[j]);
      newNormals.push_back(getNormal(i, normals[j]));
    }
    
    //create a triangles for them;
    for (int j = 0; j < lastPoints.size() - 1; j++){
      vertexList.push_back(glm::vec3(lastPoints[j]));
      //push the normal, the normal is just the direction from the center to the point;
      //assuming for now, its always the Y axis;
      normalList.push_back(lastNormals[j]);
      //normalList.push_back(getNormal(center, lastPoints[j]));,
      //uv is just well...u is just the rotation, v is the height
      uvList.push_back(getUV(true, sample,i, lastPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j]));
      normalList.push_back(newNormals[j]);
      //normalList.push_back(getNormal(center, newPoints[j]));
      uvList.push_back(getUV(false, sample, i, newPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(lastPoints[j+1]));
      normalList.push_back(lastNormals[j+1]);
      //normalList.push_back(getNormal(center, lastPoints[j + 1]));
      uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j]));
      normalList.push_back(newNormals[j]);
      //normalList.push_back(getNormal(center, newPoints[j]));
      uvList.push_back(getUV(false, sample, i, newPoints[j], maxHeight));

      vertexList.push_back(glm::vec3(lastPoints[j+1]));
      normalList.push_back(lastNormals[j+1]);
      //normalList.push_back(getNormal(center, lastPoints[j + 1]));
      uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

      vertexList.push_back(glm::vec3(newPoints[j+1]));
      normalList.push_back(newNormals[j+1]);
      //normalList.push_back(getNormal(center, newPoints[j + 1]));
      uvList.push_back(getUV(false, sample, i, newPoints[j + 1], maxHeight));
    }

    lastPoints = newPoints;
    lastNormals = newNormals;
  }

  //the last point goback to origin;
  //create a triangles for them;
  for (int j = 0; j < lastPoints.size() - 1; j++){
    vertexList.push_back(glm::vec3(lastPoints[j]));
    normalList.push_back(lastNormals[j]);
    //normalList.push_back(getNormal(center, lastPoints[j]));
    //uv is just well...u is just the rotation, v is the height
    uvList.push_back(getUV(true, sample, i, lastPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j]));
    normalList.push_back(normals[j]);
    //normalList.push_back(getNormal(center, oriPoints[j]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(lastPoints[j+1]));
    normalList.push_back(lastNormals[j+1]);
    //normalList.push_back(getNormal(center, lastPoints[j + 1]));
    uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j]));
    normalList.push_back(normals[j]);
    //normalList.push_back(getNormal(center, oriPoints[j]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j], maxHeight));

    vertexList.push_back(glm::vec3(lastPoints[j+1]));
    normalList.push_back(lastNormals[j+1]);
    //normalList.push_back(getNormal(center, lastPoints[j + 1]));
    uvList.push_back(getUV(true, sample, i, lastPoints[j + 1], maxHeight));

    vertexList.push_back(glm::vec3(oriPoints[j+1]));
    normalList.push_back(normals[j+1]);
    //normalList.push_back(getNormal(center, oriPoints[j + 1]));
    uvList.push_back(getUV(false, sample, i, oriPoints[j + 1], maxHeight));
  }
}
