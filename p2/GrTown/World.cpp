#include "World.h"
#include "Utilities\Texture.H"
#include "Utilities\ShaderTools.H"
#include <vector>






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

