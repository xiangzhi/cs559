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

float Draw::drawCardinalQuad(float t, Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p4){
	Pnt3f m0 = (p3 - p1) * (1 - t);
	Pnt3f m1 = (p4 - p2) * (1 - t);
	
	float length = 0.01;
	float distance = 0;
	for (float i = 0; i < 1; i += length){
		Pnt3f pt = calCardinalPoints(i, p2, p3, m0, m1);
		Pnt3f nextPt = calCardinalPoints(i + length, p2, p3, m0, m1);

		glBegin(GL_LINES);
		drawVertex(pt);
		drawVertex(nextPt);

		glEnd();
		
		//get a vector for them
		Pnt3f orPt = nextPt - pt;
		//normalize the direction vector;
		distance += nextPt.distance(pt);
	}
	
	return distance;
}

vector<float> Draw::drawTrack(TrainView *tv, bool doingShadow){

	//draw the line linearly
	int size = tv->world->points.size();
	if (tv->tw->splineBrowser->value() == 1){
		for (int i = 0; i < tv->world->points.size(); i++){
			glBegin(GL_LINES);
			glVertex3f(tv->world->points[i].pos.x, tv->world->points[i].pos.y, tv->world->points[i].pos.z);
			glVertex3f(tv->world->points[(i + 1) % size].pos.x, tv->world->points[(i + 1) % size].pos.y, tv->world->points[(i + 1) % size].pos.z);
			glEnd();

			//get distance
			distanceList.push_back(tv->world->points[i].pos.distance(tv->world->points[(i + 1) % size].pos));
		}
	}
	else{
		//cardinal quad
		for (int i = 0; i < tv->world->points.size(); i++){

			//interpolate the orientation vector
			float u = tv->world->trainU;

			Pnt3f orPt = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
			glPushMatrix();
			//alignOrientation(orPt);
			distanceList.push_back(drawCardinalQuad(tv->tw->tension->value(), tv->world->points[(i - 1 + size) % size].pos, tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
				tv->world->points[(i + 2) % size].pos));

			glPopMatrix();
		}
	}
	
	return distanceList;
}


Pnt3f getNextPoint(TrainView *tv, float u, int i){
	Pnt3f pt;
	int size = tv->world->points.size();
	if (tv->tw->splineBrowser->value() == 1){
		//linearly
		pt = (1 - u) * tv->world->points[i].pos + (u)*tv->world->points[(i + 1) % size].pos;
	}
	else{
		//cardinal
		pt = getSingleCardinalPoint(tv->tw->tension->value(), u, tv->world->points[(i - 1 + size) % size].pos,
			tv->world->points[(i) % size].pos, tv->world->points[(i + 1) % size].pos,
			tv->world->points[(i + 2) % size].pos);
	}
	return pt;

}

vector<Pnt3f> getInformation(TrainView *tv, float& u, int& i, int& size, Pnt3f& lst, float DISTANCE){ 
	//get the point where the car should appear on the curve

	//find the next Point with a certain distance from the last point, u on i;
	Pnt3f pt;
	Pnt3f last = lst;
	float distance = 0;
	do{
		pt = getNextPoint(tv, u, i);
		u -= 0.009;
		//make sure it can go to the next segment
		if (u < 0){
			u += 1;
			i = (i - 1 + size) % size;
		}

		distance += (lst.distance(pt));
		lst = pt;
	} while (distance < DISTANCE);
	
	//get the orientation vector
	Pnt3f orPt = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;


	//get the next point
	u = u + 0.01;
	//get the next point
	Pnt3f pt2 = getNextPoint(tv, u, i);
	//using the second vector to get the direction vector
	Pnt3f dirPt = pt2 - pt;
	//normalize the direction vector;
	dirPt.normalize();

	vector<Pnt3f> list;
	list.push_back(pt);
	list.push_back(orPt);
	list.push_back(dirPt);
	return list;
}

void Draw::drawTrain(TrainView *tv, bool doingShadow){


	drawBuildingModel(0, 0, 10);
	drawBuildingModel(100, 0, 20);
	drawBuildingModel(0, 100, 50);

	glPushMatrix();
	//move up so that it floats in the ear
	//glTranslatef(0, 5, 0);

	//get the u value
	float u = tv->world->trainU;
	//get the segment of the curve
	int i = tv->world->trainPoint;
	//get the total number of curve
	int size = tv->world->points.size();
	//get the point where the car should appear on the curves
	Pnt3f pt = getNextPoint(tv, u, i);
	//interpolate the orientation vector
	Pnt3f orPt = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;

	//get the next point
	float nextU = u + 0.01;
	int nextI = i;
	if (nextU > 1){
		nextU -= 1;
		nextI = (i + 1) % size;
	}

	
	Pnt3f pt2 = getNextPoint(tv, nextU, nextI);
	//get a direcitonal vector from the two points;
	Pnt3f dirPt = pt2 - pt;
	//normalize the direction vector;
	//dirPt.normalize();
	
	//draw the front of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(pt.x, pt.y, pt.z);
	alignObjectIn3D(dirPt, orPt);
	//draw the front car
	drawFrontCar();
	glPopMatrix();
	
	//draw the middle cars
	int carNum = tv->tw->carNum->value();
	
	for (int j = 0; j < carNum; j++){
		//get a list of parameter for the next car
		
		vector<Pnt3f> list; 
		if (j == 0){
			list = getInformation(tv, u, i, size, pt, 5);
		}
		else{
			list = getInformation(tv, u, i, size, pt, 7);
		}

		//draw the end of the train
		glPushMatrix();
		//move and rotate in the correct direction
		glTranslatef(pt.x, pt.y, pt.z);
		alignObjectIn3D(dirPt, orPt);
		//draw the train model
		drawMiddleCar();
		glPopMatrix();
	}

	//get a list of parameter for the last car
	vector<Pnt3f> list = getInformation(tv, u, i, size, pt, 7);
	//draw the end of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(pt.x, pt.y, pt.z);
	alignObjectIn3D(dirPt, orPt);
	//draw the train model
	drawBackCar();
	glPopMatrix();

	glPopMatrix();
}

vector<Pnt3f> Draw::getLookingPoints(TrainView *tv){
	float u = tv->world->trainU;
	int i = tv->world->trainPoint;
	int size = tv->world->points.size();
	Pnt3f eyeVector = getNextPoint(tv, u, i);
	//where is up;
	//get next point to get a vector
	u = u + 0.01;
	if (u > 1){
		u -= 1;
		i = (i + 1) % size;
	}
	//where to look next
	Pnt3f pt2 = getNextPoint(tv, u, i);
	//where is up
	Pnt3f upVector = (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
	vector<Pnt3f> list;
	list.push_back(eyeVector);
	list.push_back(pt2);
	list.push_back(upVector);
	return list;
}

