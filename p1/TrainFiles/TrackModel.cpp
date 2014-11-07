#include "TrackModel.h"

std::vector<Pnt3f> TrackModel::pointList1;
std::vector<Pnt3f> TrackModel::pointList2;

TrackModel::TrackModel()
{
}


TrackModel::~TrackModel()
{
}

void TrackModel::drawSimple(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow){

	if (!shadow){
		glColor3ub(142, 142, 142);
	}
	
	pointList2.push_back(p1);
	pointList2.push_back(p2);
}

void TrackModel::drawTrackBottom(Pnt3f p1, Pnt3f p2, Pnt3f orPt, bool shadow){

	if (!shadow){
		glColor3ub(117, 65, 52);
	}

	//get a directional vector
	Pnt3f dirPt = p2 - p1;

	Pnt3f pVec = ((dirPt * orPt) * 5) + dirPt;
	Pnt3f npVec = ((dirPt * orPt) * -5) + dirPt;

	//glTranslatef(pVec.x, pVec.y, pVec.z);
	glPushMatrix();
	glTranslatef(0, -3, 0);
	glBegin(GL_POLYGON);
	//std::cout << pVec << std::endl;
	//std::cout << npVec << std::endl;

	Pnt3f diff = p2 - p1;
	glVertex3d(-5, 0, 0);
	glVertex3d(npVec.x, 0, npVec.z);
	glVertex3d(5, 0, 0);
	glVertex3d(pVec.x,0,pVec.z);
	glVertex3d(-5, 0, 0);
	glEnd();

	glPopMatrix();

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

void TrackModel::draw(Pnt3f p1, Pnt3f p2, bool shadow){

	//get difference between p2 and p1;
	Pnt3f diff = p2 - p1;
	
	drawVertex(p1);
	drawVertex(p2);
	glEnd();

	/*

	if (!shadow){
		glColor3b(0, 0, 0);
	}
	//drawCube(3, 0, 0, 1);

	if (!shadow){
		glColor3ub(0, 225, 225);
	}
	drawCube(15, 0, 0, 0.5);
	*/
}