#include "Draw.h"

Draw::Draw()
{
}


Draw::~Draw()
{
}

void drawVertex(Pnt3f p){
	glVertex3f(p.x, p.y, p.z);
}

Pnt3f calBezierQuadPoints(float u, Pnt3f p1, Pnt3f p2, Pnt3f p3){
	return ((p1 * ((1 - u) * (1 - u))) + (2 * (1 - u) * u * p2) + ((u * u) * p3));
}

void drawBezierQuad(Pnt3f p1, Pnt3f p2, Pnt3f p3){
	glBegin(GL_POINTS);
	for (float i = 0; i <= 1; i += 0.05){
		drawVertex(calBezierQuadPoints(i, p1, p2, p3));
	}
	glEnd();
}

Pnt3f calCardinalPoints(float t, Pnt3f p1, Pnt3f p2, Pnt3f m0, Pnt3f m1){
	float ttt = t * t * t;
	float tt = t * t;
	Pnt3f v1 = (2 * ttt - 3 * tt + 1) * p1;
	Pnt3f v2 = (ttt - 2 * tt + t) * m0;
	Pnt3f v3 = (-2 * ttt + 3 * tt) * p2;
	Pnt3f v4 = (ttt - tt) * m1;
	return v1 + v2 + v3 + v4;
}

Pnt3f getSingleCardinalPoint(float t, float u, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4){
	Pnt3f m0 = (p3 - p1) * (1 - t);
	Pnt3f m1 = (p4 - p2) * (1 - t);
	return calCardinalPoints(u, p2, p3, m0, m1);
}

void Draw::drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4){
	Pnt3f m0 = (p3 - p1) * (1 - t);
	Pnt3f m1 = (p4 - p2) * (1 - t);
	glBegin(GL_LINES);
	float length = 0.01;
	for (float i = 0; i < 1; i += length){
		Pnt3f pt = calCardinalPoints(i, p2, p3, m0, m1);
		drawVertex(pt);
		drawVertex(calCardinalPoints(i + length, p2, p3, m0, m1));
	}
	glEnd();
}


void Draw::drawTrack(TrainView *tv, bool doingShadow){
	//just to make sure it works
	/*
	for (int i = 0; i < tv->world->points.size(); i++){
		std::cout << "point: " << i << std::endl;
		std::cout << "pos:" << tv->world->points[i].pos;
		std::cout << "ord:" << tv->world->points[i].orient;
	}
	*/
	//draw the line linearly
	/*
	
	for (int i = 0; i < tv->world->points.size(); i++){
		glBegin(GL_LINES);
			glVertex3f(tv->world->points[i].pos.x, tv->world->points[i].pos.y, tv->world->points[i].pos.z);
			glVertex3f(tv->world->points[(i+1) %size].pos.x, tv->world->points[(i+1) %size].pos.y, tv->world->points[(i+1) %size].pos.z);
		glEnd();
	}
	**/
	int size = tv->world->points.size();
	for (int i = 0; i < tv->world->points.size(); i++){
		drawCardinalQuad(0, tv->world->points[i].pos, tv->world->points[(i + 1) % size].pos, tv->world->points[(i + 2) % size].pos,
			tv->world->points[(i + 3) % size].pos);
	}
	
	//drawBezierQuad(tv->world->points[0].pos, tv->world->points[1].pos, tv->world->points[2].pos);
	//instead of a cardinal, let's make a bezier curve

}

void drawCube(Pnt3f p1, Pnt3f dirPt, Pnt3f orPt, float l){
	glPushMatrix();

	glTranslatef(p1.x, p1.y, p1.z);

	//rotate it
	


	/*
	float theta2 = -radiansToDegrees(acos(orPt.y));
	glRotatef(theta2, 0, 0, 1);
	*/
	/*
	//rotate the train so it faces forward
	float theta11 = 180 - radiansToDegrees(atan2(dirPt.z, dirPt.x));
	glRotatef(theta11, 0, 1, 0);
	
	
	float theta22 = -radiansToDegrees(acos(dirPt.y));
	glRotatef(theta22, 0, 0, 1);
	*/
	
	float theta1 = radiansToDegrees(atan2(orPt.z, orPt.x));
	glRotatef(theta1, 0, 1, 0);
	
	float theta2 = -radiansToDegrees(acos(orPt.y));
	glRotatef(theta2, 1, 0, 0);
	
	
	float theta1 = -radiansToDegrees(atan2(orPt.y, orPt.x));
	glRotatef(theta1, 0, 1, 0);
	float theta2 = -radiansToDegrees(atan2(orPt.z, orPt.y));
	glRotatef(theta2, 0, 1, 0);
	**/

	//draw front
	glBegin(GL_QUADS);
	glVertex3f(-l, -l, -l);
	glVertex3f(l , -l, -l);
	glVertex3f(l, l , -l);
	glVertex3f(-l, l, -l);
	glEnd();
	
	//drawRightSide
	glBegin(GL_QUADS);
	glVertex3f(-l, -l, -l);
	glVertex3f(-l, -l, l);
	glVertex3f(-l, l, l);
	glVertex3f(-l, l, -l);
	glEnd();

	//drawLeftSide
	glBegin(GL_QUADS);
	glVertex3f(l, -l, -l);
	glVertex3f(l, -l, l);
	glVertex3f(l, l, l);
	glVertex3f(l, l, -l);
	glEnd();
	
	//drawTop Side
	glBegin(GL_QUADS);
	glVertex3f(-l, -l, -l);
	glVertex3f(-l, -l, l);
	glVertex3f(l, -l, l);
	glVertex3f(l, -l, -l);
	glEnd();

	//draw Bottom
	glBegin(GL_QUADS);
	glVertex3f(-l, l, -l);
	glVertex3f(-l, l, l);
	glVertex3f(l, l, l);
	glVertex3f(l, l, -l);
	glEnd();


	//draw Back Side
	glBegin(GL_QUADS);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-l, -l, l);
	glVertex3f(l, -l, l);
	glVertex3f(l, l, l);
	glVertex3f(-l, l, l);
	glEnd();


	glPopMatrix();
}

void drawCubeAround(Pnt3f pt, float l){
	glBegin(GL_QUADS);

	Pnt3f temp = pt;
	temp.x += l;
	temp.y += l;
	drawVertex(temp);
	temp.x -= (l * 2);
	drawVertex(temp);
	temp.y -= (l * 2);
	drawVertex(temp);
	temp.x += (l * 2);
	drawVertex(temp);



	glEnd();
}

void Draw::drawTrain(TrainView *tv, bool doingShadow){
	float u = tv->world->trainU;
	int i = tv->world->trainPoint;
	int size = tv->world->points.size();
	Pnt3f pt = getSingleCardinalPoint(0,u,tv->world->points[(i - 1 + size)%size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
		tv->world->points[(i + 2) % size].pos);


	Pnt3f orPt = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
	std::cout << "ORT POS 2:" << tv->world->points[2].orient << std::endl;
	std::cout << "ORT POS 3:" << tv->world->points[3].orient << std::endl;
	std::cout << "ORT POS 1:" << tv->world->points[1].orient << std::endl;
	std::cout << "ORT POS 0:" << tv->world->points[0].orient << std::endl;

	//get next point to get a vector
	u = u + tv->world->speed;
	if (u > 1){
		u = 0;
		i = (i + 1) % size;
	}

	Pnt3f pt2 = getSingleCardinalPoint(0, u, tv->world->points[(i - 1 + size) % size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
		tv->world->points[(i + 2) % size].pos);
	//get a vector for them
	pt2 = pt2 - pt;
	std::cout << "POINT" << pt2;
	//normalize the direction vector;
	pt2.normalize();
	/*
	Pnt3f orPt = getSingleCardinalPoint(0, u, tv->world->points[i].orient, tv->world->points[(i + 1) % size].orient, tv->world->points[(i + 2) % size].orient,
		tv->world->points[(i + 3) % size].orient);
	*/

	std::cout << "ORT" << orPt << "i:" << i <<  std::endl;
	std::cout << "ORT POS" << tv->world->points[0].orient << std::endl;

	pt.y += 10;
	
	glPushMatrix();

	drawCube(pt, pt2, orPt, 5);
	glPopMatrix();
	//drawCubeAround(pt, 10);
}




