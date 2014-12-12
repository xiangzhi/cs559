#include "World.h"

using std::vector;

// stuff in the world
vector<GrObjectVBO*> theVBOobjects;
//static variables that exist between calls;
static GLuint landVertexBuffer;
static GLuint landNormalBuffer;
static GLuint landUVBuffer;
static GLuint landTangentBuffer;
static GLuint landBiTangentBuffer;
static GLuint landShaderID;
static Texture * landT;
static Texture * landNormalT;


// remember, these function have some responsibilities defined in the
// header... 
// for now these do simple things. in the future, maybe they will draw
// more nicely
void drawSky(DrawingState* st)
{
	// figure out the sky color...
  glm::vec3 skyColor;
	if (st->timeOfDay < 5) skyColor = glm::vec3(0, 0, .2f);
  else if (st->timeOfDay < 8) skyColor = glm::vec3(.2f, .2f, .8f);
  else if (st->timeOfDay < 16) skyColor = glm::vec3(.7f, .7f, 1);
  else if (st->timeOfDay < 19) skyColor = glm::vec3(.2f, .2f, .8f);
  else skyColor = glm::vec3(0, 0, .2f);

	glClearColor(skyColor.x,skyColor.y,skyColor.z,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);
}


void drawSkyBoxDome(glm::vec3 sun, float light, glm::mat4 PV, glm::vec3 camPos){
  static int vertexNum;
  static GLuint skyVertexBuffer;
  static GLuint skyNormalBuffer;
  static GLuint skyUVBuffer;
  static GLuint skyShaderID;
  static Texture * skyTexture;
  static int skyVertexNum;
  static bool first = true;
  //create a MVP where the object always move to camera's position
  glm::mat4 MVP = PV * glm::translate(glm::mat4(1.0f), glm::vec3(camPos.x, 0, camPos.z));

  if (first){
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> uvList;

    vertexList.push_back(glm::vec3(7000, 0, 0));
    vertexList.push_back(glm::vec3(6000, 500, 0));
    vertexList.push_back(glm::vec3(5000, 1000, 0));
    vertexList.push_back(glm::vec3(2000, 2500, 0));
    vertexList.push_back(glm::vec3(1000, 4500, 0));
    vertexList.push_back(glm::vec3(500, 6500, 0));
    vertexList.push_back(glm::vec3(0, 7000, 0));

    surfaceOfRevolution(glm::vec3(0, 1, 0), 0.5, vertexList, normalList, uvList);

    bindToArrayBuffer(skyVertexBuffer, vertexList);
    bindToArrayBuffer(skyNormalBuffer, normalList);
    bindToArrayBuffer(skyUVBuffer, uvList);

    skyShaderID = loadShader("sameVertex.glsl", "skyBoxFragment.glsl");
    skyTexture = fetchTexture("skyBox2.jpg", true, true);
    skyVertexNum = vertexList.size();
    first = false;

  }
  
  
  
  skyTexture->bind();
  GLuint sunID = glGetUniformLocation(skyShaderID, "sunDirection");
  GLuint sampleLoc = glGetUniformLocation(skyShaderID, "textureInput");
  GLuint MatrixID = glGetUniformLocation(skyShaderID, "MVP");
  GLuint lightID = glGetUniformLocation(skyShaderID, "lightInput");

  glBindBuffer(GL_ARRAY_BUFFER, skyVertexBuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
    );

  glBindBuffer(GL_ARRAY_BUFFER, skyUVBuffer);
  glVertexAttribPointer(
    2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    2,                                // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );

  glUniform1i(sampleLoc, skyTexture->texName);

  //bind normal
  glBindBuffer(GL_ARRAY_BUFFER, skyNormalBuffer);
  glVertexAttribPointer(
    3,                                // attribute. No particular reason for 1, but must match the layout in the shader.
    3,                                // size
    GL_FLOAT,                         // type
    GL_TRUE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
    );
  //get

  //get location of sun
  // glUniform1i(sampleLoc, textureId);
  glUseProgram(skyShaderID);

  //glUniform3fv(sunID,3 * sizeof(float), (float*)glm::vec3(0, 1, 0));
  glUniform3f(sunID, sun.x, sun.y, sun.z);
  glUniform1f(lightID, light);
  //Send Uniform Values to shader


  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
  //draw the box
  glDrawArrays(GL_TRIANGLES, 0, skyVertexNum);
  //unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);

}

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



void initializeEarth(){
  //Earth vertices
  std::vector<glm::vec3> vertexList;
  vertexList.push_back(glm::vec3(-20000, -1,  20000));
  vertexList.push_back(glm::vec3( 20000, -1,  20000));
  vertexList.push_back(glm::vec3( 20000, -1, -20000));
  vertexList.push_back(glm::vec3(-20000, -1,  20000));
  vertexList.push_back(glm::vec3(-20000, -1, -20000));
  vertexList.push_back(glm::vec3( 20000, -1, -20000));
  //Earth normal
  std::vector<glm::vec3> normalList;
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  normalList.push_back(glm::vec3(0, 1, 0));
  //Earth UV list
  std::vector<glm::vec2> uvList;
  uvList.push_back(glm::vec2(0, 0));
  uvList.push_back(glm::vec2(50, 0));
  uvList.push_back(glm::vec2(50, 50));
  uvList.push_back(glm::vec2(0, 0));
  uvList.push_back(glm::vec2(0, 50));
  uvList.push_back(glm::vec2(50, 50));

  //store the tangent and bi-tangent list
  std::vector<glm::vec3> tangentList;
  std::vector<glm::vec3> biTangentList;

  //create the tangent and biTangent list for the normal mapping
  //for the Earth texture
  createNormalMapping(vertexList, normalList, uvList, tangentList, biTangentList);

  //bind all the list to their respective buffer
  bindToArrayBuffer(landVertexBuffer, vertexList);
  bindToArrayBuffer(landNormalBuffer, normalList);
  bindToArrayBuffer(landUVBuffer, uvList);
  bindToArrayBuffer(landTangentBuffer, tangentList);
  bindToArrayBuffer(landBiTangentBuffer, biTangentList);
  //load shader
  landShaderID = loadShader("EarthVertex.glsl", "EarthFragment.glsl");
  //fetch both the lego and normal 
  landT = fetchTexture("lego.jpg", true, true);
  glActiveTexture(GL_TEXTURE1);
  landNormalT = fetchTexture("normalLego.jpg", true, true, GL_TEXTURE1);
  glActiveTexture(GL_TEXTURE0);
}

void drawLand(glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::vec3 sun, float light){

  //draw the land
  //bind each buffer
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
    2,                                // attribute. No particular reason for 2, but must match the layout in the shader.
    3,                                // size
    GL_FLOAT,                         // type
    GL_TRUE,                          // normalized?
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

  //TEXTURES
  //bind the normal texture
  glActiveTexture(GL_TEXTURE0);
  landT->bind();
  GLuint sampleLoc = glGetUniformLocation(landShaderID, "textureInput");
  glUniform1i(sampleLoc, 0);
  //bind the normal mapping
  glActiveTexture(GL_TEXTURE1);
  landNormalT->bind();
  sampleLoc = glGetUniformLocation(landShaderID, "normalInput");
  glUniform1i(sampleLoc, 1);
  //glUniform1i(sampleLoc, landNormalT->texName);

  //load all the uniform values
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
  
  //draw the array
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //disable shader
  glUseProgram(0);
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
}

float calculateSunLight(DrawingState* drst){
  //lighting calculations
  return 1.05 - (abs(12 - drst->timeOfDay) / 12.0);
}

glm::vec3 calculateSunDirection(DrawingState* drst){
  glm::vec3 sun(0, 0, 0);
  if (drst->timeOfDay > 6 && drst->timeOfDay < 18){
    float angle = (drst->timeOfDay - 6) / 3 * 45;
    sun = glm::rotateZ(glm::vec3(1, 0, 0), angle);
  }
  return sun;
}
