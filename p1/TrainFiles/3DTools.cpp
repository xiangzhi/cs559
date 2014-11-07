#pragma once

/*
* 3DTools.cpp
* helpful functions for 3D manipulations
* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/

#include "3DTools.h"

void alignObjectIn3D(Pnt3f dirPt, Pnt3f orPt){

	//normalize the xAxis
	Pnt3f xAxis = dirPt;
	xAxis.normalize();
	//get the zAxis
	Pnt3f zAxis = orPt * xAxis;
	zAxis.normalize();
	//get the real Yxis;
	Pnt3f yAxis = xAxis * zAxis;
	yAxis.normalize();
	//yAxis = -1 * yAxis;

	//initialize matrix with zeros
	GLfloat mat44[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			mat44[i][j] = 0;
		}
	}
	//the corner one
	mat44[3][3] = 1;

	//put everything into the matrix;
	mat44[0][0] = xAxis.x;
	mat44[0][1] = xAxis.y;
	mat44[0][2] = xAxis.z;

	mat44[1][0] = yAxis.x;
	mat44[1][1] = yAxis.y;
	mat44[1][2] = yAxis.z;

	mat44[2][0] = zAxis.x;
	mat44[2][1] = zAxis.y;
	mat44[2][2] = zAxis.z;

	//make sure it multiply the matrix
	glMultMatrixf(*mat44);
}