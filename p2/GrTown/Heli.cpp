#include "Heli.h"
#include "ObjectLoader.h"
#include "BHLights.h"



class HeliBlade : public GrObjectVBO{
  void initialize(){
    std::vector<glm::vec3> vertexList;
    std::vector<glm::vec3> normalList;
    std::vector<glm::vec2> uvList;

    vertexList.push_back(glm::vec3(0, 0, 0));
    vertexList.push_back(glm::vec3(10, 0, 0));
    vertexList.push_back(glm::vec3(10, 1, 0));
    vertexList.push_back(glm::vec3(0, 1, 0));

    surfaceOfRevolution(glm::vec3(0, 1, 0), 10, vertexList, normalList, uvList);

    bindToArrayBuffer(vertexBuffer, vertexList);
    bindToArrayBuffer(normalBuffer, normalList);
    bindToArrayBuffer(textureBuffer, uvList);

    shaderID = loadShader("heliVertex.glsl", "heliBladeFragment.glsl");
    vertexNum = vertexList.size();
  }
  void preDraw(DrawingState* drst){
    transform = glm::scale(scale);
    transform = glm::rotate(transform, ry, glm::vec3(0, 1, 0));
    transform = glm::translate(pos) * transform;
  }

  void simulateUntil(unsigned long time){
    ry = ((int)ry + 1)% 360;
  }

};


Heli::Heli()
{
}

Heli::Heli(GrObjectVBO* _track) :GrObjectVBO("BlackHawak")
{
  track = _track;
  GrObjectVBO* light = new BHLights(this, _track);
  light->parent = this;
  children.push_back(light);  
  GrObjectVBO* blades = new HeliBlade();
  blades->parent = this;
  blades->scale = glm::vec3(10, 1, 10);
  blades->pos = glm::vec3(0, 38, 0);
  children.push_back(blades);
}

Heli::~Heli()
{
}

void Heli::initialize(){
  // Read our .obj file
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec2 > uvs;
  std::vector< glm::vec3 > normals; // Won't be used at the moment.
  bool res = loadOBJ("blackHawk.obj.model", vertices, uvs, normals);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

  glGenBuffers(1, &textureBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

  //shader
  char* err;
  //transform = glm::translate(glm::vec3(0, 30, 0)) * glm::scale(glm::vec3(5,5,5));
  type = GL_TRIANGLES;

  vertexNum = vertices.size() / 3;
  shaderID = loadShader("heliVertex.glsl", "heliFragment.glsl", err);
}

void Heli::preDraw(DrawingState* drst){

  localTransform = glm::scale(glm::vec3(60, 60, 60));
  transform = glm::mat4(1.0f);
  pos.x = track->pos.x + 10;
  pos.y = 1000;
  pos.z = track->pos.z + 10;
  
  to = pos;
  if (glm::distance(from, pos) > 700){
    from.x = pos.x + 100;
    from.z = pos.z + 100;
    from.y = pos.y + 200;
  }
  
  transform = glm::translate(pos) * transform;
}

glm::mat4 Heli::getCamera(){
  glm::vec3 eye(pos.x + 10, pos.y + 10, pos.z);
  glm::vec3 target(track->pos);
  return glm::lookAt(
    eye,
    target,
    glm::vec3(0, 1, 0)
    );
}

void Heli::simulateUntil(unsigned long time){
  for (int i = 0; i < children.size(); i++){
    children[i]->simulateUntil(time);
  }
}