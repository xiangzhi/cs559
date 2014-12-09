/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
//
// updated Fall 2006 - more testing code
// updated Fall 2008 - switched some internal stuff (no more linked lists)

#include "GrTown_PCH.H"
#include "GraphicsTownUI.H"
#include "FlyCamera.H"
#include "GrWorld.H"
#include "DrawUtils.H"

// for setting up shader paths and textures
#include "Utilities/ShaderTools.H"
#include "Utilities/Texture.H"


#include "Building.h"
#include "Sphere.h"
#include "Model.h"
#include "Billboards.h"
#include "Dino.h"
#include "Bob.h"
#include "BHLights.h"

#include "Examples\Suburbs.H"
// define this to get 2 cars that always turn
// #define TESTCARS

const int numCars = 100;
const int nGrids = 5;

int main(int /*argc*/, char** /*argv*/)
{ 

  // put in some texture paths - look all over the place
	texturePaths.push_back(".");
	texturePaths.push_back("..");
	texturePaths.push_back("Textures/signs");
	texturePaths.push_back("../Textures/signs");
	texturePaths.push_back("Textures/Textures");
	texturePaths.push_back("../Textures/Textures");
	texturePaths.push_back("Textures/Objects");
	texturePaths.push_back("../Textures/Objects");

	shaderPaths.push_back("Shaders");
	shaderPaths.push_back("../Shaders");

  // *****************************************************************
  //  Make your town here

  // first, some really simple things

  // cubes are particularly easy since they set their own position
  // we need to raise the cube since we're giving the position of the
  // center 
  // add some more stuff
  GrObject* o1 = new Church;
  o1->interesting = true;
  o1->laX = 0; o1->laY = 0; o1->laZ = 0;
  o1->lfX = -50; o1->lfY = 100; o1->lfZ = 300;
  add(o1, -100, 0, 100, pi / 2.f);

  GrObjectVBO* b = new Building();
  add(b);
  b->laX = 0;
  b->laY = 0;
  b->laZ = 0;
  b->lfX = 100;
  b->lfY = 50;
  b->lfZ = 100;
  b->interesting = true;


  for (int i = 0; i < 100; i++){
    GrObjectVBO* billboard = new Billboards();

    float x = (4000 - rand() % 2001);
    if (rand() % 2 == 0){
      x *= -1;
    }

    float z = rand() % 4000;
    if (rand() % 2 == 0){
      z *= -1;
    }
    
    billboard->pos = glm::vec3(x,0,z);

    add(billboard);
  }

  for (int i = 0; i < 100; i++){
    GrObjectVBO* billboard = new Billboards();

    float z = (4000 - rand() % 2001);
    if (rand() % 2 == 0){
      z *= -1;
    }

    float x = rand() % 4000;
    if (rand() % 2 == 0){
      x *= -1;
    }

    billboard->pos = glm::vec3(x, 0, z);

    add(billboard);
  }


  GrObjectVBO* dino = new Dino();
  add(dino);
  GrObjectVBO* bob = new Bob();
  bob->interesting = true;
  add(bob);
  GrObjectVBO* super = new Model(bob);
  super->interesting = true;
  add(super);
  //GrObjectVBO* s = new Sphere();
  //add(s);


  GrObjectVBO* obj = new BHLights();
  add(obj);

  // *****************************************************************
  // now make a UI
  FlyCamera* fc = new FlyCamera;
  Map* m = new Map;

  add(fc);
  add(m);

  GraphicsTownUI grTown;
  grTown.make_window();
  grTown.townView->defaultCamera = fc;
  grTown.window->show();

  Fl::run();
  return 1;
}
// $Header: /p/course/cs559-gleicher/private/CVS/GrTown/Main.cpp,v 1.8 2009/11/10 22:40:03 gleicher Exp $
