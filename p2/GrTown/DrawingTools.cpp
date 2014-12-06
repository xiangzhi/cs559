#include "DrawingTools.h"


void bindToArrayBuffer(GLuint& ptrToBuffer, double sizeOfLocation, void* ptrToLocation){
  glGenBuffers(1, &ptrToBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, ptrToBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeOfLocation, ptrToLocation, GL_STATIC_DRAW);
}