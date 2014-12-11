#include "Model.h"
#include "ObjectLoader.h"
#include "BHLights.h"

Model::Model()
{
}

Model::Model(GrObjectVBO* _track) :GrObjectVBO("BlackHawak")
{
  track = _track;
  GrObjectVBO* light = new BHLights(this, _track);
  light->parent = this;
  children.push_back(light);
}

Model::~Model()
{
}

void Model::initialize(){
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
  shaderID = loadShader("simpleVertex.glsl", "simpleFragment.glsl", err);
}

void Model::preDraw(DrawingState* drst){

  localTransform = glm::scale(glm::vec3(60, 60, 60));
  transform = glm::mat4(1.0f);
  pos.x = track->pos.x + 10;
  pos.y = 800;
  pos.z = track->pos.z + 10;
  transform = glm::translate(pos) * transform;
}

glm::mat4 Model::getCamera(){
  glm::vec3 eye(pos.x + 10, pos.y + 10, pos.z);
  glm::vec3 target(track->pos);
  return glm::lookAt(
    eye,
    target,
    glm::vec3(0, 1, 0)
    );
}