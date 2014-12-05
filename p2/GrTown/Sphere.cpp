#include "Sphere.h"
#include <math.h>

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}

void addPoint(std::vector<double> &lst, glm::vec3 v){
  lst.push_back(v.x);
  lst.push_back(v.y);
  lst.push_back(v.z);
}


void Sphere::initialize(){

  float points[] =
  {
    0.0f, -1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    -1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.0f
  };

  int indices[] =
  {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 1,
    1, 5, 2,
    2, 5, 3,
    3, 5, 4,
    4, 5, 1
  };

  std::vector<int> index;
  std::vector<float> p;
  int vCount = 6;

  for (int i = 0; i < 18; i++){
    p.push_back(points[i]);
  }

  for (int i = 0; i < 24; i++){
    index.push_back(indices[i]);

  }

  for (int i = 0; i < 24; i += 3){
    int i1, i2, i3;

    i1 = indices[i];
    i2 = indices[i + 1];
    i3 = indices[i + 2];

    int temp = i1 * 3;
    glm::vec3 p1(points[temp], points[temp + 1], points[temp + 2]);
    temp = i2 * 3;
    glm::vec3 p2(points[temp], points[temp + 1], points[temp + 2]);
    temp = i3 * 3;
    glm::vec3 p3(points[temp], points[temp + 1], points[temp + 2]);


    glm::vec3 p4 = (p1 + p2 + p3);
    p4 = p4 / glm::vec3(3,3,3);

    p.push_back(p4.x);
    p.push_back(p4.y);
    p.push_back(p4.z);

    int newI = vCount;
    vCount++;

    index.push_back(i1);
    index.push_back(i2);
    index.push_back(newI);

    index.push_back(i1);
    index.push_back(i3);
    index.push_back(newI);

    index.push_back(i2);
    index.push_back(i3);
    index.push_back(newI);
  }



  // fill "indices" as needed
  
  // Generate a buffer for the indices
  glGenBuffers(1, &indexBuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(int), &index[0], GL_STATIC_DRAW);
  useIndex = true;
  indexNum = index.size();

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, p.size() * sizeof(float), &p[0], GL_STATIC_DRAW);


  //shader
  char* err;
  transform = glm::translate(glm::vec3(0,20,100)) * glm::scale(glm::vec3(10, 10, 10));
  type = GL_TRIANGLES;

  vertexNum = 6;
  shaderID = loadShader("simpleVertex.glsl", "simpleFragment.glsl", err);
}