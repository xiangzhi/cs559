#pragma once
#include "PhysicModel.h"

float PhysicModel::pEnergy = 0;
float PhysicModel::kEnergy = 0;
float PhysicModel::friction = 1;
float PhysicModel::gravity = 5;
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

	speed += (-1 * dirVec.y * gravity);

	if (speed - friction > speedN){
		speed -= friction;
	}
	
	if (speedN > speed){
		speed = speedN;
	}

	if (speed <= 0.5){
		speed = 0.5;
	}

	return speed;
}

