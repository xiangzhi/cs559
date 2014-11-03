#include "TrainModel.h"

void drawTrainModel(){
	//nothing yet 


	glScalef(2, 2, 2);
	glScalef(1.5, 1.25, 1);

	drawMiddleCar();

	//glTranslatef(-1.5, 0, 0);
	/*
	//top of the train
	glBegin(GL_QUADS);
	glVertex3f(1, 0, -1);
	glVertex3f(1, 0, 1);
	glVertex3f(-1, 0, 1);
	glVertex3f(-1, 0, -1);
	glEnd();

	//the front of the train
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, -1);
	glVertex3f(1.5, -0.5, -1);
	glVertex3f(1.5, -0.5, 1);
	glEnd();

	//the back of the train
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1, 0, 1);
	glVertex3f(-1, 0, -1);
	glVertex3f(-1.5, -0.5, -1);
	glVertex3f(-1.5, -0.5, 1);
	glEnd();

	//the right side of the train
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1.5, -0.5, 1);
	glVertex3f(-1.5, -0.5, 1);
	glEnd();

	//the left side of the train
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-1, 0, -1);
	glVertex3f(1, 0, -1);
	glVertex3f(1.5, -0.5, -1);
	glVertex3f(-1.5, -0.5, -1);
	glEnd();
	*/
}

void _drawMiddleCar(){

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

}

void drawMiddleCar(){
	glPushMatrix();
	glScalef(3, 4, 3);
	_drawMiddleCar();
	glPopMatrix();
}

void _drawFrontCar(){
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
	glColor3f(0.0, 0.0, 0.0);
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
	glColor3f(1.0, 0.0, 0.0);
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
}
void drawFrontCar(){
	glPushMatrix();
	glScalef(3, 4, 3);
	_drawFrontCar();
	glPopMatrix();
}

void drawBackCar(){
	glPushMatrix();
	glTranslatef(-3, 0, 0);
	glScalef(-1, 1, -1);
	glScalef(3, 4, 3);
	_drawFrontCar();
	glPopMatrix();
}