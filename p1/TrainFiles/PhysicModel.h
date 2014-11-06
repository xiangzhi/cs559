#pragma once

#include "Utilities\Pnt3f.h"

class PhysicModel
{
public:
	static float getVelocity(float speed, Pnt3f dirVec, Pnt3f pt);
	static float pEnergy;
	static float kEnergy;
	static float gravity;
	static float friction;
	static float speed;
};
