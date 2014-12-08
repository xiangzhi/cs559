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

glm::vec2 getUV(float rotation, glm::vec4 point, float maxHeight){
  return glm::vec2(rotation, point.y / maxHeight);
}

void surfaceOfRevolution(glm::vec3 center, std::vector<float>& vertexlist, std::vector<float> *  normallist, std::vector<float>  * uvlist){

  //make the rotation matrix;

  //convert the list into a list of glm::vec3
  std::vector<glm::vec4> oriPoints;
  std::vector<glm::vec4> lastPoints;
  std::vector<glm::vec4> finalVertexList;
  std::vector<glm::vec3> finalNormalList;
  std::vector<glm::vec2> finalUVList;

  float maxHeight = -1;
  float minHeight = 0;

  for (int i = 0; i < vertexlist.size(); i += 3){
    glm::vec4 temp(vertexlist[i], vertexlist[i + 1], vertexlist[i + 2],0);
    oriPoints.push_back(temp);
    lastPoints.push_back(temp);
    if (temp.y > maxHeight){
      maxHeight = temp.y;
    }
  }
  int sample = 90;
  float i = sample;
  for (i = sample; i < 360; i += sample){
    std::vector<glm::vec4> newPoints;
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), i, center);
    for (int j = 0; j < oriPoints.size(); j++){
      newPoints.push_back(rotMat * oriPoints[j]);
    }

    //create a triangles for them;
    for (int j = 0; j < lastPoints.size() - 1; j++){
      finalVertexList.push_back(lastPoints[j]);
      //push the normal, the normal is just the direction from the center to the point;
      //assuming for now, its always the Y axis;
      finalNormalList.push_back(getNormal(center, lastPoints[j]));
      //uv is just well...u is just the rotation, v is the height
      finalUVList.push_back(getUV(i / 360, lastPoints[j], maxHeight));

      finalVertexList.push_back(newPoints[j]);
      finalNormalList.push_back(getNormal(center, newPoints[j]));
      finalUVList.push_back(getUV(i / 360, newPoints[j], maxHeight));

      finalVertexList.push_back(lastPoints[j + 1]);
      finalNormalList.push_back(getNormal(center, lastPoints[j+1]));
      finalUVList.push_back(getUV(i / 360, lastPoints[j + 1], maxHeight));

      finalVertexList.push_back(newPoints[j]);
      finalNormalList.push_back(getNormal(center, newPoints[j]));
      finalUVList.push_back(getUV(i / 360, newPoints[j], maxHeight));

      finalVertexList.push_back(lastPoints[j+1]);
      finalNormalList.push_back(getNormal(center, lastPoints[j + 1]));
      finalUVList.push_back(getUV(i / 360, lastPoints[j + 1], maxHeight));

      finalVertexList.push_back(newPoints[j+1]);
      finalNormalList.push_back(getNormal(center, newPoints[j + 1]));
      finalUVList.push_back(getUV(i / 360, newPoints[j + 1], maxHeight));
    }

    lastPoints = newPoints;
  }

  //the last point goback to origin;
  //create a triangles for them;
  for (int j = 0; j < lastPoints.size() - 1; j++){
    finalVertexList.push_back(lastPoints[j]);
    //push the normal, the normal is just the direction from the center to the point;
    //assuming for now, its always the Y axis;
    finalNormalList.push_back(getNormal(center, lastPoints[j]));
    //uv is just well...u is just the rotation, v is the height
    finalUVList.push_back(getUV(i / 360, lastPoints[j], maxHeight));

    finalVertexList.push_back(oriPoints[j]);
    finalNormalList.push_back(getNormal(center, oriPoints[j]));
    finalUVList.push_back(getUV(0, oriPoints[j], maxHeight));

    finalVertexList.push_back(lastPoints[j + 1]);
    finalNormalList.push_back(getNormal(center, lastPoints[j + 1]));
    finalUVList.push_back(getUV(i / 360, lastPoints[j + 1], maxHeight));

    finalVertexList.push_back(oriPoints[j]);
    finalNormalList.push_back(getNormal(center, oriPoints[j]));
    finalUVList.push_back(getUV(i / 360, oriPoints[j], maxHeight));

    finalVertexList.push_back(lastPoints[j + 1]);
    finalNormalList.push_back(getNormal(center, lastPoints[j + 1]));
    finalUVList.push_back(getUV(i / 360, lastPoints[j + 1], maxHeight));

    finalVertexList.push_back(oriPoints[j + 1]);
    finalNormalList.push_back(getNormal(center, oriPoints[j + 1]));
    finalUVList.push_back(getUV(i / 360, oriPoints[j + 1], maxHeight));
  }

  vertexlist.clear();
  //done;
  //push back everything to the vertexList;
  for (int i = 0; i < finalVertexList.size(); i++){
    vertexlist.push_back(finalVertexList[i].x);
    vertexlist.push_back(finalVertexList[i].y);
    vertexlist.push_back(finalVertexList[i].z);
    normallist->push_back(finalNormalList[i].x);
    normallist->push_back(finalNormalList[i].y);
    normallist->push_back(finalNormalList[i].z);
  }

  for (int i = 0; i < finalUVList.size(); i++){
    uvlist->push_back(finalUVList[i].x);
    uvlist->push_back(finalUVList[i].y);
  }
}





#include"DrawingTools.h"
#include "Utilities\ShaderTools.H"

void surfaceOfRevolutionTest(glm::mat4 VP, glm::vec3 sun){

  static bool first = true;
  static GLuint shaderID;
  static GLuint vertexBuffer;
  static GLuint normalBuffer;
  static GLuint textureBuffer;
  static int size;
  if (first){

    float points[] = {
      1, 0, 0,
      1, 1, 0,
      1, 2, 0,
      0.85,2.25,0,
      0.65,2.5,0,
      0.35,2.75,0,
      0, 3, 0
    };

    int listSize = (sizeof(points) / sizeof(float));

    std::vector<float> vList;
    std::vector<float> uList;
    std::vector<float> nList;
    listToVector(points, listSize, vList);

    surfaceOfRevolution(glm::vec3(0, 1, 0), vList, &nList, &uList);

    //load it into the buffer
    bindToArrayBuffer(vertexBuffer,vList.size() * sizeof(float), &vList[0]);
    bindToArrayBuffer(normalBuffer, nList.size() * sizeof(float), &nList[0]);

    size = vList.size();
    char *err;
    shaderID = loadShader("simpleVertex.glsl", "simpleFragment.glsl", err);
    first = false;
  }

  //draw the vList;
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  //get the location of the values outside, since we reusing shader
  GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
  //GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
  GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
  //draw the four sides;

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(
    3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glVertexAttribPointer(
    2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    2,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glm::mat4 MVP = VP * glm::scale(glm::mat4(1.0f),glm::vec3(5,5,5));

  //get location of sun
  //glUniform1i(sampleLoc, t->texName);
  glUseProgram(shaderID);

  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  //draw the box
  glDrawArrays(GL_TRIANGLES, 0, size / 3);
  //unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

}