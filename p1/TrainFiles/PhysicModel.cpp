#pragma once
#include "PhysicModel.h"

float PhysicModel::pEnergy = 0;
float PhysicModel::kEnergy = 0;
float PhysicModel::friction = 2;
float PhysicModel::gravity = 9.8;
float PhysicModel::speed = 0;


float PhysicModel::getVelocity(float speed, Pnt3f dirVec, Pnt3f pt){

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


	speed -= friction;

	if (speed <= 0.5){
		speed = 0.5;
	}

	return speed;
}

