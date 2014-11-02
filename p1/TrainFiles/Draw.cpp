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

vector<Pnt3f> buildCoordinateSystem(Pnt3f p1, Pnt3f p){
	//p = p1 + p;
	Pnt3f temp = p;
	temp.x += 1;
	//temp cross p
	Pnt3f zAxis = temp * p;
	Pnt3f xAxis = p * zAxis;

	xAxis.normalize();
	zAxis.normalize();
	p.normalize();

	std::vector<Pnt3f> list;
	list.push_back(xAxis);
	list.push_back(p);
	list.push_back(zAxis);

	return list;
}

void alignOrientation(Pnt3f p1, Pnt3f orPt){
	//align the cube to the orientation
	GLfloat mat44[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			mat44[i][j] = 0;
		}
	}
	mat44[3][3] = 1;
	vector<Pnt3f> list = buildCoordinateSystem(p1, orPt);
	for (int i = 0; i < 3; i++){
		mat44[0][i] = list[i].x;
		mat44[1][i] = list[i].y;
		mat44[2][i] = list[i].z;
	}
	glMultMatrixf(*mat44);
}

void alignDirection(Pnt3f dirPt){
	//rotate the train so it faces forward
	float theta1 = 180 - radiansToDegrees(atan2(dirPt.z, dirPt.x));
	glRotatef(theta1, 0, 1, 0);
	float theta2 = -radiansToDegrees(acos(dirPt.y));
	glRotatef(theta2, 0, 1, 0);
}



void drawMonorailTrack(Pnt3f p, Pnt3f orPt){
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);

	glScalef(2, 2, 2);

	//alignOrientation(p, orPt);
	alignDirection(orPt);

	glBegin(GL_QUADS);
	glVertex3f(1.5, 0, 0);
	glVertex3f(1, -0.1, 0);
	glVertex3f(-1, -0.1, 0);
	glVertex3f(-1.5, 0, 0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(2, 0.1, 0);
	glVertex3f(2.5, 0.1, 0);
	glVertex3f(2, -0.1, 0);
	glVertex3f(1, -0.1, 0);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(-2, 0.1, 0);
	glVertex3f(-2.5, 0.1, 0);
	glVertex3f(-2, -0.1, 0);
	glVertex3f(-1, -0.1, 0);

	//top
	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	glVertex3f(-2, 0.5, 0);
	glVertex3f(-2.5, 0.5, 0);
	glVertex3f(-2.5, 0.5, -0.5);
	glVertex3f(-2, 0.5, -0.5);
	glEnd();
	//top
	
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);
	glVertex3f(1.5, 0, 0);
	glVertex3f(1.5, 0, -0.5);
	glVertex3f(-1.5, 0, -0.5);
	glVertex3f(-1.5, 0, 0);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(-2, 0.5, 0);
	glVertex3f(-2, 0.5, -0.5);
	glVertex3f(-1.5, 0, -0.5);
	glVertex3f(-1.5, 0, 0);
	glEnd();


	glPopMatrix();
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

float Draw::drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4){
	Pnt3f m0 = (p3 - p1) * (1 - t);
	Pnt3f m1 = (p4 - p2) * (1 - t);
	
	float length = 0.01;
	float distance = 0;
	for (float i = 0; i < 1; i += length){
		Pnt3f pt = calCardinalPoints(i, p2, p3, m0, m1);
		Pnt3f nextPt = calCardinalPoints(i + length, p2, p3, m0, m1);
		/*
		glBegin(GL_LINES);
		drawVertex(pt);
		drawVertex(nextPt);

		glEnd();
		*/

		//get a vector for them
		Pnt3f orPt = nextPt - pt;
		//normalize the direction vector;
		drawMonorailTrack(pt,orPt);
		distance += nextPt.distance(pt);
	}
	
	return distance;
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
	vector<float> distanceList;
	for (int i = 0; i < tv->world->points.size(); i++){
		distanceList.push_back(drawCardinalQuad(0, tv->world->points[(i - 1 + size) % size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
			tv->world->points[(i + 2) % size].pos));
	}
	/*
	for (int i = 0; i < distanceList.size(); i++){
		std::cout << "i:" << i << " " << distanceList[i] << std::endl;
	}
	*/

	//drawBezierQuad(tv->world->points[0].pos, tv->world->points[1].pos, tv->world->points[2].pos);
	//instead of a cardinal, let's make a bezier curve

}





void drawCubes(Pnt3f p1, Pnt3f p2){
	//drawFront
	glBegin(GL_QUADS);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p1.z);
	glVertex3f(p1.x, p2.y, p1.z);
	glEnd();

	//drawTop
	glBegin(GL_QUADS);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p1.x, p1.y, p2.z);
	glVertex3f(p2.x, p1.y, p2.z);
	glVertex3f(p2.x, p1.y, p1.z);
	glEnd();
}


void drawCube(Pnt3f p1, Pnt3f dirPt, Pnt3f orPt, float l){
	glPushMatrix();

	glTranslatef(p1.x, p1.y, p1.z);

	//align the cube to the orientation
	GLfloat mat44[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			mat44[i][j] = 0;
		}
	}
	mat44[3][3] = 1;
	vector<Pnt3f> list = buildCoordinateSystem(p1, orPt);
	for (int i = 0; i < 3; i++){
		mat44[0][i] = list[i].x;
		mat44[1][i] = list[i].y;
		mat44[2][i] = list[i].z;
	}
	glMultMatrixf(*mat44);
	

	//rotate the train so it faces forward
	float theta1 = 180 - radiansToDegrees(atan2(dirPt.z, dirPt.x));
	glRotatef(theta1, 0, 1, 0);
	float theta2 = -radiansToDegrees(acos(dirPt.y));
	glRotatef(theta2, 0, 0, 1);

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
	//normalize the direction vector;
	pt2.normalize();
	/*
	Pnt3f orPt = getSingleCardinalPoint(0, u, tv->world->points[i].orient, tv->world->points[(i + 1) % size].orient, tv->world->points[(i + 2) % size].orient,
		tv->world->points[(i + 3) % size].orient);
	*/



	pt.y += 10;
	
	glPushMatrix();

	drawCube(pt, pt2, orPt, 5);
	glPopMatrix();
	glPushMatrix();
	//drawMonorailTrack(pt, orPt);
		glPopMatrix();
	//drawCubeAround(pt, 10);
}

vector<Pnt3f> Draw::getLookingPoints(TrainView *tv){
	float u = tv->world->trainU;
	int i = tv->world->trainPoint;
	int size = tv->world->points.size();
	Pnt3f eyeVector = getSingleCardinalPoint(0, u, tv->world->points[(i - 1 + size) % size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
		tv->world->points[(i + 2) % size].pos);
	//where is up;
	//get next point to get a vector
	u = u + tv->world->speed;
	if (u > 1){
		u = 0;
		i = (i + 1) % size;
	}
	//where to look next
	Pnt3f pt2 = getSingleCardinalPoint(0, u, tv->world->points[(i - 1 + size) % size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
		tv->world->points[(i + 2) % size].pos);
	//where is up
	Pnt3f upVector = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;

	vector<Pnt3f> list;
	list.push_back(eyeVector);
	list.push_back(pt2);
	list.push_back(upVector);
	return list;
}

