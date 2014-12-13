/// GraphicsTown2006 - CS559 Sample Code
// written by Michael Gleicher - originally written Fall, 2000
//
// updated Fall 2006 - more testing code
// updated Fall 2008 - switched some internal stuff (no more linked lists)

#include "GrTown_PCH.H"
#include "GraphicsTownUI.H"
#include "FlyCamera.H"
#include <time.h>

// for setting up shader paths ands textures
#include "Utilities/ShaderTools.H"
#include "Utilities/Texture.H"

#include "Heli.h"
#include "Billboards.h"
#include "Bob.h"
#include "BHLights.h"
#include "City.h"
#include "LegoBrick.h"
#include "APC.h"
#include "SoRDemo.h"
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

  //set randomness;
  srand(time(NULL));

  // *****************************************************************
  //  Make your town here

  GrObjectVBO* bob = new Bob();
  bob->name = "Bob - The jumping Monster";
  bob->ridable = true;
  bob->rideAbove = 500;
  add(bob);

  GrObjectVBO* heli = new Heli(bob);
  heli->name = "BlackHawk-1";
  heli->ridable = true;
  heli->rideAbove = 500;
  add(heli);

  GrObjectVBO* city = new City();
  ((City*) city)->destroyer = bob;
  add(city);
  city->name = "City View";
  city->to = glm::vec3(0, 0, 0);
  city->from = glm::vec3(1000, 1000, 1000);
  city->interesting = true;

  GrObjectVBO* billboard = new Billboards();
  billboard->name = "TeddyBear Billboard";
  billboard->pos = glm::vec3(-2500, -200, 1500);
  billboard->to = glm::vec3(-3000, 400, 2000);
  billboard->from = glm::vec3(2000, 600, -1500);
  billboard->interesting = true;
  add(billboard);

  GrObjectVBO* apc = new APC(0);
  apc->name = "Loop Sample - 0 div.";
  apc->scale = (glm::vec3(0.3, 0.3, 0.3));
  apc->ry = 45;
  apc->pos = glm::vec3(1500, 0, 1500);
  apc->interesting = true;
  apc->to = glm::vec3(1500, 0, 1500);
  apc->from = glm::vec3(1600, 200, 2000);
  add(apc);

  apc = new APC(1);
  apc->name = "Loop Sample - 1 div.";
  apc->scale = (glm::vec3(0.3, 0.3, 0.3));
  apc->ry = 45;
  apc->pos = glm::vec3(1900, 0, 1500);
  apc->interesting = true;
  apc->to = glm::vec3(1900, 0, 1500);
  apc->from = glm::vec3(1400, 200, 1800);
  add(apc);

  apc = new APC(2);
  apc->name = "Loop Sample - 2 div.";
  apc->scale = (glm::vec3(0.3, 0.3, 0.3));
  apc->ry = 45;
  apc->pos = glm::vec3(2300, 0, 1500);
  apc->interesting = true;
  apc->to = glm::vec3(2300, 0, 1500);
  apc->from = glm::vec3(1800, 200, 1800);
  add(apc);
  
  //Surface of Rotation Demo
  GrObjectVBO* sr1 = new SoRDemo(10);
  sr1->name = "surface of rotation 10";
  sr1->scale = glm::vec3(4, 4, 4);
  sr1->pos = glm::vec3(2300, 0, 1900);
  sr1->interesting = true;
  sr1->to = glm::vec3(2300,40, 1900);
  sr1->from = glm::vec3(2600, 200, 1600);
  add(sr1);

  sr1 = new SoRDemo(30);
  sr1->name = "surface of rotation 30";
  sr1->scale = glm::vec3(4, 4, 4);
  sr1->pos = glm::vec3(2700, 0, 1900);
  sr1->interesting = true;
  sr1->to = glm::vec3(2700, 40, 1900);
  sr1->from = glm::vec3(3000, 200, 1600);
  add(sr1);

  sr1 = new SoRDemo(90);
  sr1->name = "surface of rotation 90";
  sr1->scale = glm::vec3(4, 4, 4);
  sr1->pos = glm::vec3(3100, 0, 1900);
  sr1->interesting = true;
  sr1->to = glm::vec3(3100, 40, 1900);
  sr1->from = glm::vec3(3400, 200, 1600);
  add(sr1);

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
