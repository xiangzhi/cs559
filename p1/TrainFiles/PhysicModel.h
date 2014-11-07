#pragma once

/*
* physicModel.h
* simple and stupid physic model;
* xiang zhi tan - cs559 - p1
*/

#include "Utilities\Pnt3f.h"



class PhysicModel
{
public:
	/* return the velocity of the object*/
	static float getVelocity(float speed, Pnt3f dirVec, Pnt3f pt);
	static float pEnergy;
	static float kEnergy;
	static float gravity;
	static float friction;
	static float speed;
};
