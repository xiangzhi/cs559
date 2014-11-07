#include "TrainModel.h"

/*
* trainModel.cpp
* draw the train model
* see .h file for what each method do
* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/

void _drawMiddleCar(bool shadow){

	//move zero to the center
	glScalef(1, 3, 1);
	glTranslatef(1, 0.25, 0);
	if (!shadow){
		glColor3ub(189, 32, 49);
	}

	//top of the car
	glBegin(GL_QUADS);
	glVertex3f(0, 0.5, -1);
	glVertex3f(0, 0.5, 1);
	glVertex3f(-2, 0.5, 1);
	glVertex3f(-2, 0.5, -1);
	glEnd();

	//front of the car
	glBegin(GL_QUADS);
	glVertex3f(0, 0.5, -1);
	glVertex3f(0, 0.5, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(0, 0, -1);
	glEnd();

	//back of the car
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.5, -1);
	glVertex3f(-2, 0.5, 1);
	glVertex3f(-2, 0, 1);
	glVertex3f(-2, 0, -1);
	glEnd();

	//sides of the car
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.5, 1);
	glVertex3f(0, 0.5, 1);
	glVertex3f(0, 0, 1);
	glVertex3f(-2, 0, 1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.5, -1);
	glVertex3f(0, 0.5, -1);
	glVertex3f(0, 0, -1);
	glVertex3f(-2, 0, -1);
	glEnd();

	//mirror on the sides
	if (!shadow){
		glColor3ub(255, 255, 255);
	}
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.35, 1.01);
	glVertex3f(0, 0.35, 1.01);
	glVertex3f(0, 0.15, 1.01);
	glVertex3f(-2, 0.15, 1.01);
	glEnd();/*
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.5, -1);
	glVertex3f(0, 0.5, -1);
	glVertex3f(0, 0, -1);
	glVertex3f(-2, 0, -1);
	glEnd();*/

}

void TrainModel::drawMiddleCar(bool doingShadow){
	glPushMatrix();
	glScalef(3, 4, 3);
	_drawMiddleCar(doingShadow);
	glPopMatrix();
}

void _drawFrontCar(bool shadow){
	//top of the train

	if (!shadow){
		glColor3ub(189, 32, 49);
	}

	

	//move the center to the middle
	
	glScalef(1, 3, 1);
	glTranslatef(1.5, 0.25, 0);
	//top of the train

	glBegin(GL_QUADS);
	glVertex3f(-1, 0.5, -1);
	glVertex3f(-1, 0.5, 1);
	glVertex3f(-3, 0.5, 1);
	glVertex3f(-3, 0.5, -1);
	glEnd();

	//the front of the train
	//top part
	glBegin(GL_QUADS);
	glVertex3f(-1, 0.5, -1);
	glVertex3f(-1, 0.5, 1);
	glVertex3f(0, 0.1, 1);
	glVertex3f(0, 0.1, -1);
	glEnd();

	//the front of the train
	//bottom part
	glBegin(GL_QUADS);
	glVertex3f(0, 0.1, 1);
	glVertex3f(0, 0.1, -1);
	glVertex3f(0, 0, -1);
	glVertex3f(0, 0, 1);
	glEnd();

	//the back of the car
	glBegin(GL_QUADS);
	glVertex3f(-3, 0.5, 1);
	glVertex3f(-3, 0.5, -1);
	glVertex3f(-3, 0, -1);
	glVertex3f(-3, 0, 1);
	glEnd();

	//the right side of the train
	glBegin(GL_POLYGON);
	glVertex3f(-3, 0, 1);
	glVertex3f(-3, 0.5, 1);
	glVertex3f(-1, 0.5, 1);
	glVertex3f(0, 0.1, 1);
	glVertex3f(0, 0, 1);
	glEnd();

	//the left side of the train
	glBegin(GL_POLYGON);
	glVertex3f(-3, 0, -1);
	glVertex3f(-3, 0.5, -1);
	glVertex3f(-1, 0.5, -1);
	glVertex3f(0, 0.1, -1);
	glVertex3f(0, 0, -1);
	glEnd();

	//the front of the train
	//mirror
	//top part
	glBegin(GL_QUADS);
	if (!shadow){
		glColor3ub(255, 255, 255);
	}
	glVertex3f(-0.85, 0.452, -1);
	glVertex3f(-0.85, 0.452, 1);
	glVertex3f(-0.1, 0.152, 1);
	glVertex3f(-0.1, 0.152, -1);
	glEnd();

	//the side of the train
	//mirror
	//side
	glBegin(GL_QUADS);
	if (!shadow){
		glColor3ub(255, 255, 255);
	}
	glVertex3f(-0.85, 0.452, -1.01);
	glVertex3f(-0.1, 0.152, -1.01);
	glVertex3f(-2, 0.152, -1.02);
	glVertex3f(-2, 0.452, -1.02);
	glEnd();

	glBegin(GL_QUADS);
	if (!shadow){
		glColor3ub(255, 255, 255);
	}
	glVertex3f(-0.85, 0.452, 1.01);
	glVertex3f(-0.1, 0.152, 1.01);
	glVertex3f(-2, 0.152, 1.02);
	glVertex3f(-2, 0.452, 1.02);
	glEnd();

}
void TrainModel::drawFrontCar(bool doingShadow){
	glPushMatrix();
	glScalef(3, 4, 3);
	_drawFrontCar(doingShadow);
	glPopMatrix();
}

void TrainModel::drawBackCar(bool doingShadow){
	glPushMatrix();
	glTranslatef(-3, 0, 0);
	glScalef(-1, 1, 1);
	glScalef(3, 4, 3);
	_drawFrontCar(doingShadow);
	glPopMatrix();
}