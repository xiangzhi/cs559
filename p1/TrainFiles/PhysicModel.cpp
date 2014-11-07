#pragma once

/*
 * simple and stupid physic model;
 * xiang zhi tan - cs559 - p1
 */

#include "PhysicModel.h"

float PhysicModel::pEnergy = 0;
float PhysicModel::kEnergy = 0;
float PhysicModel::friction = 0.5;
float PhysicModel::gravity = 4;
float PhysicModel::speed = 0;


float PhysicModel::getVelocity(float speedN, Pnt3f dirVec, Pnt3f pt){
	/*
	float changeInP = pt.y - pEnergy;
	pEnergy = pt.y;
	
	kEnergy -= changeInP;
	if (kEnergy < 0){
		kEnergy = 0;
	}

	//cap the maximum kinetic energy??
	if (kEnergy > 10){
		kEnergy = 10;
	}

	speed += kEnergy;
	//if going up hill
	if (changeInP > 0){
		speed -= changeInP;
	}
	*/
	
	//increase the speed by gravity * direction
	speed += (-1 * dirVec.y * gravity);

	//if the speed given by user is faster than speed after friction
	//then we don't decrease friction
	if ((speed - friction) > speedN){
		speed -= friction;
	}
	
	//always at least maintain the speed given by the user
	//in this physic model, the speed of the user is the minimum speed
	//we could model such that the speed is less than the user's speed
	//but who wants that?
	if (speedN > speed){
		speed = speedN;
	}
	//return the correct speed
	return speed;
}

