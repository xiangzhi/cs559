#include "Billboards.h"
#include "DrawingState.H"
#include <gtc\type_ptr.hpp>

Billboards::Billboards()
{
}


Billboards::~Billboards()
{
}


void sendVec3ToShader(GLuint shaderID, std::string name, glm::vec3 v){
  GLuint location = glGetUniformLocation(shaderID, name.c_str());
  //pass our MVP to shader
  glUniform3fv(location, 1, glm::value_ptr(v));
}

void Billboards::initialize(){

  std::vector<glm::vec3> vList;
  vList.push_back(glm::vec3(-10, 0, 0));
  vList.push_back(glm::vec3(10, 0, 0)); 
  vList.push_back(glm::vec3(10, 10,0));

  vList.push_back(glm::vec3(-10, 0, 0));
  vList.push_back(glm::vec3(-10, 10, 0));
  vList.push_back(glm::vec3(10, 10, 0));

  std::vector<glm::vec2> uList;

  uList.push_back(glm::vec2(1, 1));
  uList.push_back(glm::vec2(0, 1));
  uList.push_back(glm::vec2(0, 0));

  uList.push_back(glm::vec2(1, 1));
  uList.push_back(glm::vec2(1, 0));
  uList.push_back(glm::vec2(0, 0));

  std::vector<glm::vec3> nList;

  nList.push_back(glm::vec3(0, 0, 1));
  nList.push_back(glm::vec3(0, 0, 1));
  nList.push_back(glm::vec3(0, 0, 1));
  nList.push_back(glm::vec3(0, 0, 1));
  nList.push_back(glm::vec3(0, 0, 1));
  nList.push_back(glm::vec3(0, 0, 1));

  /*
  int size = 8;
  int xDecrease = 5;
  int yIncrease = 10;
  for (int i = 0; i < size; i++){
    float y = (i * yIncrease) + 10;
    float x = (size * xDecrease) - i * xDecrease;

    vList.push_back(glm::vec3(-x, y, -100));
    vList.push_back(glm::vec3(x, y, -100));
    vList.push_back(glm::vec3(x, y + 10, -100));

    vList.push_back(glm::vec3(-x, y, -100));
    vList.push_back(glm::vec3(-x, y + yIncrease, -100));
    vList.push_back(glm::vec3(x, y + yIncrease, -100));
  }


  std::vector<glm::vec3> nList;
  for (int i = 0; i < vList.size(); i++){
    nList.push_back(glm::vec3(0, 0, 1));
  }

  std::vector<glm::vec2> uList;

  uList.push_back(glm::vec2(0.5, 0));
  uList.push_back(glm::vec2(1, 0));
  uList.push_back(glm::vec2(1, 0.5));

  uList.push_back(glm::vec2(0.5, 0));
  uList.push_back(glm::vec2(0.5, 0.5));
  uList.push_back(glm::vec2(1.0, 0.5));

  for (int i = 0; i < vList.size() - 6 ; i += 6){
    uList.push_back(glm::vec2(0, 0.5));
    uList.push_back(glm::vec2(0.5, 0.5));
    uList.push_back(glm::vec2(0.5, 1));
    uList.push_back(glm::vec2(0, 0.5));
    uList.push_back(glm::vec2(0.5, 1));
    uList.push_back(glm::vec2(0.5, 1));
  }
  */
  float normals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
  };

  float texturePoints[] = {
    //bottom
    0.5, 0,
    1, 0,
    1, 0.5,
    0.5,0,
    0.5,0.5,
    1,0.5
  };


  /*
  // Generate a buffer for the indices
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_STATIC_DRAW);
  useIndex = true;
  indexNum = index.size();
  */

  bindToArrayBuffer(vertexBuffer, vList);
  bindToArrayBuffer(textureBuffer, uList);
  bindToArrayBuffer(normalBuffer,nList);

  char* err;
  shaderID = loadShader("billboardVertex.glsl", "billboardFragment.glsl", err);
  vertexNum = vList.size();

  transform = glm::scale(glm::vec3(150,150,150));
  transform = glm::translate(pos) * transform;


  //t = fetchTexture("testTree.png", true, true);
  t = loadPNG("bear-transparent.png");
  useTexture = true;

}

/**
void Billboards::realDraw(DrawingState* drst, glm::mat4 proj, glm::mat4 view, glm::mat4 model){

  //if for some reason, something change
  if (redraw){
    //reinitialize the buffer and put stuff in.
    initialize();
    redraw = false;
  }

  //generate the PVM matrux
  glm::mat4 MVP = proj * view * model;

  //enable the vertexAttribArray
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  //bind vertex buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  //bind colorBuffer
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(
    1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );


  //make sure the texture exist
  if (useTexture){
    //bind before every draw
    t->bind();
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    glVertexAttribPointer(
      2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
      2,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
      );

    GLuint sampleLoc = glGetUniformLocation(shaderID, "textureInput");
    glUniform1i(sampleLoc, t->texName);
  }

  //bind normalBuffer
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(
    3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glUseProgram(shaderID);
  //get the location of MVP in shader
  GLuint MatrixID = glGetUniformLocation(shaderID, "MVP");
  //pass our MVP to shader
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  //lighting calculations
  float light = 0;
  //day time
  glm::vec3 sun(0, 0, 0);
  if (drst->timeOfDay > 6 && drst->timeOfDay < 18){
    light = 6 - abs(12 - drst->timeOfDay);
    float angle = (drst->timeOfDay - 6) / 3 * 45;
    sun = glm::rotateZ(glm::vec3(1, 0, 0), angle);
  }

  //get location of sun
  GLuint sunID = glGetUniformLocation(shaderID, "sunDirection");
  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);

  if (useIndex){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(
      type,
      indexNum,
      GL_UNSIGNED_INT,
      (void*)0
      );
  }
  else{
    glDrawArrays(type, 0, vertexNum);
  }

  //disable everything
  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);
}
*/
void Billboards::runAttribute(glm::mat4 proj, glm::mat4 view, glm::mat4 model){
  //billboard stuff

  glm::vec4 center = glm::vec4(pos.x, pos.y, pos.z, 0);
  center = model * center;
  glm::vec3 camRight = glm::vec3(view[0][0], view[1][0], view[2][0]);
  glm::vec3 camUp = glm::vec3(view[0][1], view[1][1], view[2][1]);


  sendVec3ToShader(shaderID, "camRight", glm::vec3(view[0][0], view[1][0], view[2][0]));
  sendVec3ToShader(shaderID, "camUp", glm::vec3(view[0][1], view[1][1], view[2][1]));
  sendVec3ToShader(shaderID, "center", glm::vec3(center));
  sendVec3ToShader(shaderID, "size", glm::vec3(40, 20, 0));

  glm::mat4 trans(1.0f);
  trans[0][0] = view[0][0];
  trans[1][0] = view[1][0];
  trans[2][0] = view[2][0];

  trans[0][1] = view[0][1];
  trans[1][1] = view[1][1];
  trans[2][1] = view[2][1];

  //get the location of MVP in shader
  GLuint MatrixID = glGetUniformLocation(shaderID, "trans");
  //pass our MVP to shader
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &trans[0][0]);
}