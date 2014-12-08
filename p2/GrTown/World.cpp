#include "World.h"
#include "Utilities\Texture.H"
#include "Utilities\ShaderTools.H"
#include <vector>


#include "SurfaceOfRevolution.h"


//store the buffers for everything
static std::vector<GLuint> bufferList;
static Texture* t[5];
static GLuint shaderID;
void initializeSkyBox(){

  //all the vertexs
  //height
  float h = 5000;
  float w = 5000;
  float l = 5000;
  float points[] = {
    -l, 0.0f, w, //front triangle 
    l, 0.0f, w,
    l, h, w,//end front traingle 1
    -l, 0.0f, w, //front traingle 2 
    -l, h, w,
    l, h, w, //end front triangle
    //right side
    l, 0.0f, w, //right side triangle 1
    l, 0.0f, -w,
    l, h, -w, //end of right side triangle;
    l, 0.0f, w, //right side triangle 2
    l, h, w,
    l, h, -w, //end of right side triangle;
    //left side
    -l, 0.0f, w, //left side triangle 1
    -l, 0.0f, -w,
    -l, h, -w, //end of left side triangle;
    -l, 0.0f, w, //left side triangle 2
    -l, h, w,
    -l, h, -w, //left of right side triangle;
    //back
    -l, 0.0f, -w, //back triangle 
    l, 0.0f, -w,
    l, h, -w,//end back traingle 1
    -l, 0.0f, -w, //back traingle 2 
    -l, h, -w,
    l, h, -w, //end back triangle
    //top
    -l, h, w,
    l, h, w,
    l, h, -w,
    -l, h, w,
    -l, h, -w,
    l, h, -w
  };

  float normalPoints[] = {
    //front
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    //right
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    //left
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    //back
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    //top
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
  };

  float texturePoints[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
  };


  for (int i = 0; i < 6; i++){

    int pointIndex = i * 18;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint textureBuffer;
    //add the points
    bindToArrayBuffer(vertexBuffer, 3 * 6 * sizeof(float), &points[pointIndex]);
    bindToArrayBuffer(normalBuffer, 3 * 6 * sizeof(float), &normalPoints[pointIndex]);
    bindToArrayBuffer(textureBuffer, sizeof(texturePoints), texturePoints);
    //add buffer address to list
    bufferList.push_back(vertexBuffer);
    bufferList.push_back(normalBuffer);
    bufferList.push_back(textureBuffer);
  }


  t[0] = fetchTexture("sky_box_side_1.jpg", false, false);
  t[1] = fetchTexture("sky_box_side_2.jpg", true, true);
  t[2] = fetchTexture("sky_box_side_3.jpg", true, true);
  t[3] = fetchTexture("sky_box_side_4.jpg", true, true);
  t[4] = fetchTexture("sky_box_top.png", true, true);

  char* err;
  shaderID = loadShader("sameVertex.glsl", "skyBoxFragment.glsl", err);
}

void drawSkyBox(glm::vec3 sun, float light, glm::mat4 MVP){
  static bool first = true;
  if (first){
    initializeSkyBox();
    first = false;
  }

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  //get the location of the values outside, since we reusing shader
  GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
  GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
  GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
  //draw the four sides;
  for (int i = 0; i < 5; i++){
    GLuint vertexBuffer = bufferList[(i * 3)];
    GLuint normalBuffer = bufferList[(i * 3) + 1];
    GLuint textureBuffer = bufferList[(i * 3) + 2];
    GLuint textureId = t[i]->texName;
    t[i]->bind();
    

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
      0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
      );

    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glVertexAttribPointer(
      2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
      );

    glUniform1i(sampleLoc, textureId);

    //bind normal
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
      3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
      );
    //get

    //get location of sun
   // glUniform1i(sampleLoc, textureId);
    glUseProgram(shaderID);

    //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
    glUniform3f(sunID, sun.x, sun.y, sun.z);

    //Send Uniform Values to shader
    

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    //draw the box
    glDrawArrays(GL_TRIANGLES, 0, 6);
    //unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
  }
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);


}

static GLuint headVertexBuffer;
static GLuint headNormalBuffer;
static GLuint headUVBuffer;

static GLuint landVertexBuffer;
static GLuint landNormalBuffer;
static GLuint landUVBuffer;
static GLuint landTangentBuffer;
static GLuint landBiTangentBuffer;

static GLuint landShaderID;
static GLuint headShaderID;
static Texture * landT;
static Texture * landNormalT;
static int headSize;



void doubleHead(std::vector<float> & list, std::vector<float> & normallist, std::vector<float> & UVlist, int x, int z){
  int size = list.size();
  for (int i = 0; i < size; i += 3){
    list.push_back(list[i] + x);
    list.push_back(list[i + 1]);
    list.push_back(list[i + 2] + z);

    normallist.push_back(normallist[i]);
    normallist.push_back(normallist[i + 1]);
    normallist.push_back(normallist[i + 2]);
  }

  size = UVlist.size();

  for (int j = 0; j < size; j += 2){
    UVlist.push_back(UVlist[j]);
    UVlist.push_back(UVlist[j+1]);

    UVlist.push_back(UVlist[j]);
    UVlist.push_back(UVlist[j + 1]);

    UVlist.push_back(UVlist[j]);
    UVlist.push_back(UVlist[j + 1]);
  }
}
bool head = false;


#include "normalMapping.h"

void initializeEarth(){
  char* err;
  if (head){
    //head
    float headPoints[] = {
      1, 0, 0,
      1, 1, 0,
      0, 1, 0
    };

    std::vector<float> vList;
    std::vector<float> uList;
    std::vector<float> nList;


    //double vList


    listToVector(headPoints, sizeof(headPoints) / sizeof(float), vList);
    surfaceOfRevolution(glm::vec3(0, 1, 0), vList, &nList, &uList);

    //doubleHead(vList, nList, uList, 80, 0);
    doubleHead(vList, nList, uList, 40, 0);
    doubleHead(vList, nList, uList, 20, 0);
    doubleHead(vList, nList, uList, 10, 0);
    //doubleHead(vList, nList, uList, 0, 80);
    doubleHead(vList, nList, uList, 0, 40);
    doubleHead(vList, nList, uList, 0, 20);
    doubleHead(vList, nList, uList, 0, 10);


    bindToArrayBuffer(headVertexBuffer, vList);
    bindToArrayBuffer(headUVBuffer, uList);
    bindToArrayBuffer(headNormalBuffer, nList);

    headSize = nList.size();
    

    headShaderID = loadShader("EarthHeadVertex.glsl", "EarthHeadFragment.glsl", err);
  }

  //all the vertexs
  float points[] = {
    -25000.0f, -1.0f, -25000.0f,
    25000.0f, -1.0f, -25000.0f,
    25000.0f, -1.0f, 25000.0f,
    -25000.0f, -1.0f, -25000.0f,
    -25000.0f, -1.0f, 25000.0f,
    25000.0f, -1.0f, 25000.0f
  };

  float normalPoints[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  };

  float UVpoints[] = {
    0.0f, 0.0f,
    50.0f, 0.0f,
    50.0f, 50.0f,
    0.0f, 0.0f,
    0.0f, 50.0f,
    50.0f, 50.0f
  };

  std::vector<glm::vec3> vertexList;
  std::vector<glm::vec3> normalList;
  std::vector<glm::vec2> uvList;
  std::vector<glm::vec3> tangentList;
  std::vector<glm::vec3> biTangentList;

  //copy values to list
  listToVector(points, sizeof(points) / sizeof(float), vertexList);
  listToVector(UVpoints, sizeof(UVpoints) / sizeof(float), uvList);
  listToVector(normalPoints, sizeof(points) / sizeof(float), normalList);

  createNormalMapping(vertexList, normalList, uvList, tangentList, biTangentList);

  bindToArrayBuffer(landVertexBuffer, sizeof(points), points);
  bindToArrayBuffer(landNormalBuffer, normalList);
  bindToArrayBuffer(landUVBuffer, uvList);
  bindToArrayBuffer(landTangentBuffer, tangentList);
  bindToArrayBuffer(landBiTangentBuffer, biTangentList);
  bindToArrayBuffer(landUVBuffer, sizeof(UVpoints), UVpoints);
  bindToArrayBuffer(landUVBuffer, sizeof(UVpoints), UVpoints);
  bindToArrayBuffer(landUVBuffer, sizeof(UVpoints), UVpoints);

  landShaderID = loadShader("EarthVertex.glsl", "EarthFragment.glsl", err);
  landT = fetchTexture("lego.jpg", true, true);
  glActiveTexture(GL_TEXTURE1);
  landNormalT = fetchTexture("normalLego.jpg", true, true, GL_TEXTURE1);
  glActiveTexture(GL_TEXTURE0);
}

void drawHead(glm::mat4 MVP, glm::vec3 sun, float light){
  //draw the vList;
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  //get the location of the values outside, since we reusing shader
  GLuint sunID = glGetUniformLocation(headShaderID, "sunDirection");
  GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
  GLuint MatrixID = glGetUniformLocation(headShaderID, "MVP");
  //draw the four sides;

  glBindBuffer(GL_ARRAY_BUFFER, headVertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, headNormalBuffer);
  glVertexAttribPointer(
    3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  //get location of sun
  glUniform1i(sampleLoc, landT->texName);
  glUseProgram(headShaderID);

  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);

  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  //draw the box
  glDrawArrays(GL_TRIANGLES, 0, headSize / 3);
  //unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
}

void drawLand(glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::vec3 sun, float light){

  //draw the land
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ARRAY_BUFFER, landVertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, landUVBuffer);
  glVertexAttribPointer(
    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, landNormalBuffer);
  glVertexAttribPointer(
    2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    3,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, landTangentBuffer);
  glVertexAttribPointer(
    3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    3,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, landBiTangentBuffer);
  glVertexAttribPointer(
    4,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    3,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );


  glUseProgram(landShaderID);

  glActiveTexture(GL_TEXTURE0);
  landT->bind();
  GLuint sampleLoc = glGetUniformLocation(landShaderID, "textureInput");
  glUniform1i(sampleLoc, 0);

  glActiveTexture(GL_TEXTURE1);
  landNormalT->bind();
  sampleLoc = glGetUniformLocation(landShaderID, "normalInput");
  glUniform1i(sampleLoc, 1);
  //glUniform1i(sampleLoc, landNormalT->texName);

  glm::mat3 m3(model);

  GLuint model3ID = glGetUniformLocation(landShaderID, "M3");
  glUniformMatrix4fv(model3ID, 1, GL_FALSE, &m3[0][0]);
  GLuint modelID = glGetUniformLocation(landShaderID, "M");
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
  GLuint viewID = glGetUniformLocation(landShaderID, "V");
  glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
  GLuint projID = glGetUniformLocation(landShaderID, "P");
  glUniformMatrix4fv(projID, 1, GL_FALSE, &proj[0][0]);
  GLuint sunID = glGetUniformLocation(landShaderID, "sunDirection");
  glUniform3f(sunID, sun.x, sun.y, sun.z);
 
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);

  //unbind texture
  //unbind texture1
  glBindTexture(GL_TEXTURE_2D, 0);
  //unbind texture0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);

}

void drawEarth(glm::mat4 proj,glm::mat4 view, glm::vec3 camPos,glm::vec3 sunDirection,float light){

  static bool first = true;
  if (first){
    initializeEarth();
    first = false;
  }
  drawLand(proj, view, glm::mat4(1.0f), sunDirection, light);
  
  if (head){

    float distance = 80;
    for (int i = 0; i < 2; i++){
      float x = ((i - 1) * distance) + camPos.x;
      for (int j = 0; j < 1; j++){
        float z = ((j - 1) * distance) + camPos.z;
        glm::mat4 trans = glm::scale(glm::mat4(1.0f), glm::vec3(6, 6, 6)) * glm::translate(glm::mat4(1.0f), glm::vec3(x, 0, z));
        glm::mat4 model = trans;
        glm::mat4 MVP = proj * view * model;
        drawHead(MVP, sunDirection, 0);
      }
    }
  }

  //only draw heads near the user

}