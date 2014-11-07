/*
 * TrackModel.cpp
 * Contains the drawing methods to draw the track
 * see .h file for what each method do
 * Xiang Zhi Tan - CS559 Fall 2014 - P1
 */

#include "TrackModel.h"

std::vector<Pnt3f> TrackModel::pointList1;
std::vector<Pnt3f> TrackModel::pointList2;

void TrackModel::drawSimple(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow){

	if (!shadow){
		glColor3ub(142, 142, 142);
	}
	
	pointList2.push_back(p1);
	pointList2.push_back(p2);
}

void TrackModel::clearIt(){
	pointList1.clear();
	pointList2.clear();
}

void TrackModel::drawDual(Pnt3f p1, Pnt3f 
	p2, Pnt3f orPt, bool shadow){

	//set thickness
	glLineWidth(2);

	if (!shadow){
		glColor3ub(142, 142, 142);
	}

	//get a directional vector
	Pnt3f dirPt = p2 - p1;
	dirPt.normalize();

	glPushMatrix();

	//get a perpendicular vector
	Pnt3f pVec = dirPt * orPt;

	pVec = pVec * 2;

	//glTranslatef(pVec.x, pVec.y, pVec.z);
	pointList1.push_back(p1 + pVec);
	pointList1.push_back(p2 + pVec);
	//draw(p1, p2, shadow);
	glPopMatrix();

	glPushMatrix();

	//reset the perpendicular vector
	pVec.normalize();
	pVec = pVec * -2;
	//glTranslatef(pVec.x, pVec.y, pVec.z);
	pointList2.push_back(p1 + pVec);
	pointList2.push_back(p2 + pVec);
	//draw(p1, p2, shadow);
	glPopMatrix();
}

void TrackModel::drawIt(){

	glLineWidth(4);

	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < pointList1.size(); i++){
		drawVertex(pointList1[i]);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < pointList2.size(); i++){
			drawVertex(pointList2[i]);
		}
	glEnd();
}

void TrackModel::drawRailTrails(Pnt3f pt, Pnt3f dirPt, Pnt3f orPt){
	glPushMatrix();
	//move to the correct point
	glTranslatef(pt.x, pt.y, pt.z);

	alignObjectIn3D(dirPt, orPt);

	glScalef(0.5, 0.5, 2);
	drawCube(0, 0, 0, 5);

	glPopMatrix();
}