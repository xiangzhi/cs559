/*
* BuildingModel.cpp
* Contains the drawing methods to draw a building
* see .h file for what each method do
* Xiang Zhi Tan - CS559 Fall 2014 - P1
*/


#include "BuildingModel.h"

void _drawCube(){
	int l = 1;
	
	glColor3f(0, 0, 0);

	//draw front
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -l);
	glVertex3f(0, l, -l);
	glVertex3f(0, l, 0);
	glEnd();

	//draw top
	glBegin(GL_QUADS);
	glVertex3f(0, l, 0);
	glVertex3f(0, l, -l);
	glVertex3f(-l, l, -l);
	glVertex3f(-l, l, 0);
	glEnd();

	//draw back
	glBegin(GL_QUADS);
	glVertex3f(-l , 0, 0);
	glVertex3f(-l, 0, -l);
	glVertex3f(-l, l, -l);
	glVertex3f(-l, l, 0);
	glEnd();

	//draw sides
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(-l, 0, 0);
	glVertex3f(-l, l, 0);
	glVertex3f(0, l, 0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0, 0, -l);
	glVertex3f(-l, 0, -l);
	glVertex3f(-l, l, -l);
	glVertex3f(0, l, -l);
	glEnd();
}

void _drawSquare(){
	//draw 
	int l = 1;
	glColor3f(1, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -l);
	glVertex3f(0, l, -l);
	glVertex3f(0, l, 0);
	glEnd();
}


void drawBuildingModel(int x, int z, float height){
	
	glPushMatrix();
	
	//draw windows;
	for (int i = 0; i < height/5; i++){
		glPushMatrix();
		glTranslatef(0.01, i * 5, -1);
		glScalef(3, 3, 3);
		_drawSquare();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.01, i * 5, -6);
		glScalef(3, 3, 3);
		_drawSquare();
		glPopMatrix();
	}
	

	glScalef(10, height, 10);
	_drawCube();
	glPopMatrix();
	
}