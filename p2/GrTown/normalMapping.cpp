#include "normalMapping.h"


void createNormalMapping(std::vector<glm::vec3>& vertexlist, std::vector<glm::vec3>& normallist, std::vector<glm::vec2>& uvlist,
  std::vector<glm::vec3> & tangents, std::vector<glm::vec3> & bitangents){

  for (int i = 0; i < vertexlist.size(); i += 3){

    // Shortcuts for vertices
    glm::vec3  v0 = vertexlist[i + 0];
    glm::vec3  v1 = vertexlist[i + 1];
    glm::vec3  v2 = vertexlist[i + 2];

    // Shortcuts for UVs
    glm::vec2  uv0 = uvlist[i + 0];
    glm::vec2  uv1 = uvlist[i + 1];
    glm::vec2  uv2 = uvlist[i + 2];

    // Edges of the triangle : postion delta
    glm::vec3 deltaPos1 = v1 - v0;
    glm::vec3 deltaPos2 = v2 - v0;

    // UV delta
    glm::vec2 deltaUV1 = uv1 - uv0;
    glm::vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

    tangents.push_back(tangent);
    tangents.push_back(tangent);
    tangents.push_back(tangent);

    // Same thing for binormals
    bitangents.push_back(bitangent);
    bitangents.push_back(bitangent);
    bitangents.push_back(bitangent);
  }
}