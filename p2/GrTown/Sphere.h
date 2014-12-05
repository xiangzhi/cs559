#pragma once
#include "GrObjectVBO.h"
#include <gtc\constants.hpp>
class Sphere :
  public GrObjectVBO
{
public:
  Sphere();
  ~Sphere();

  void initialize();
};

