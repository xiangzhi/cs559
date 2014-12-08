#include "Model.h"
#include "ObjectLoader.h"

Model::Model()
{
}


Model::~Model()
{
}

void Model::initialize(){
  // Read our .obj file
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec2 > uvs;
  std::vector< glm::vec3 > normals; // Won't be used at the moment.
  bool res = loadOBJ("superman.obj", vertices, uvs, normals);

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

  glGenBuffers(1, &textureBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);

  //shader
  char* err;
  transform = glm::translate(glm::vec3(0, 30, 0)) * glm::scale(glm::vec3(5,5,5));
  type = GL_TRIANGLES;

  vertexNum = vertices.size() / 3;
  shaderID = loadShader("simpleVertex.glsl", "simpleFragment.glsl", err);


}