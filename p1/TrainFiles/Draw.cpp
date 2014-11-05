#include "Draw.h"

Draw::Draw()
{
}


Draw::~Draw()
{
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

void uValidation(TrainView *tv, float& u, int& i){
	//make sure the u is valid, if not, move it back to front
	while (u > tv->world->points.size()){
		u -= tv->world->points.size();
	}

	//get which line it is controlling
	
	i = 0;
	while (u > 1){
		u -= 1;
		i++;
	}
	
}

/* get the point on the track from given u and i*/
Pnt3f getPointOnTrack(TrainView *tv, float u){

	int i = 0;
	uValidation(tv, u, i);

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

/* get the point on the track from given u and i*/
Pnt3f getPointOnTrack(TrainView *tv, float u, int i){
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

Pnt3f getOrientationVector(TrainView *tv, float u){
	int i;
	uValidation(tv, u, i);
	int size = tv->world->points.size();
	return (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
}

Pnt3f getOrientationVector(TrainView *tv, float u, int i){
	int size = tv->world->points.size();
	return (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
}

Pnt3f getDirectionVector(Pnt3f pt, TrainView *tv, float u, int i){
	//get the next point
	float nextU = u + 0.01;
	int nextI = i;
	if (nextU > 1){
		nextU -= 1;
		nextI = (i + 1) % tv->world->points.size();
	}
	//get the next small point on the line
	Pnt3f pt2 = getPointOnTrack(tv, nextU, nextI);
	//get a direcitonal vector from the two points;
	return pt2 - pt;
}

Pnt3f getDirectionVector(Pnt3f pt, TrainView *tv, float u){
	//get the next small point on the line
	Pnt3f pt2 = getPointOnTrack(tv, u + 0.01);
	//get a direcitonal vector from the two points;
	return pt2 - pt;
}

typedef void(*drawCallback)(Pnt3f pt, Pnt3f dirPt, Pnt3f orPt);

vector<float> buildArcLengthCurveTableI(TrainView *tv){
	vector<float> arcTable;
	float totalDistance = 0;
	Pnt3f lstPt;
	for (int i = 0; i < tv->distanceList.size(); i++){
		for (int u = 0; u < 100; u++){
			float rU = u * 0.01;
			Pnt3f pt = getPointOnTrack(tv, rU, i);
			if (u == 0 && i == 0){
				totalDistance = 0;
			}
			else{
				totalDistance += (pt.distance(lstPt));
			}
			lstPt = pt;
			arcTable.push_back(totalDistance);
		}
	}
	std::cout << "done" << std::endl;
	std::cout << "size: " << arcTable.size() << std::endl;
	return arcTable;
}

void drawRailTrails(Pnt3f pt, Pnt3f dirPt, Pnt3f orPt){
	glPushMatrix();
	//move to the correct point
	glTranslatef(pt.x, pt.y, pt.z);

	alignObjectIn3D(dirPt, orPt);

	glScalef(0.5, 0.5, 2);
	drawCube(0, 0, 0, 5);

	glPopMatrix();

	//draw support?
	/*
	glPushMatrix();

	glBegin(GL_LINES);
	drawVertex(pt);
	glVertex3f(pt.x, 0, pt.z);
	glEnd();
	glPopMatrix();
	*/
}

void buildObjectAtInterval(TrainView *tv, drawCallback callback, float interval){

	vector<float> arcTable = tv->arcLengthTable;

	float curPoint = 0;
	float nextPoint = 0;
	float total = arcTable[arcTable.size() - 1];
	float index = 0;
	while (nextPoint < total && index < arcTable.size()){
		while (nextPoint < arcTable[index]){

			int num = 0;
			int temp = index;


			while (temp >= 100){
				num += 1;
				temp -= 100;
			}

			float u = temp * 0.01;

			Pnt3f pt = getPointOnTrack(tv, u, num);
			Pnt3f dirPt = getDirectionVector(pt, tv, u, num);
			Pnt3f orPt = getOrientationVector(tv, u, num);

			//call the drawing callback
			callback(pt,dirPt,orPt);
			nextPoint += interval;
		}
		index++;
	}
}

float getPointFromArcLength(vector<float> arcLengthTable, float distance){
	float last = 0;
	for (int i = 0; i < arcLengthTable.size(); i++){
		if (distance < arcLengthTable[i]){
			float diff = (distance - last) / (arcLengthTable[i] - last);
			return (i + (diff * 1)) * 0.01;
		}
		last = arcLengthTable[i];
	}
	return -1;
}

float getDistanceFromArcLength(TrainView *tv, float u){
	vector<float> arcLengthTable = tv->arcLengthTable;
	while(u < 0){
		u += tv->world->points.size();
	}

	float upper = ceil(u * 100);
	float lower = floor(u * 100);
	if (upper >=  tv->world->points.size() * 100){
		upper = tv->world->points.size() * 100 - 1;
	}
	if (lower > tv->world->points.size() * 100){
		lower = 0;
	}
	float diff = (u * 100) - lower;
	float d1 = arcLengthTable[upper];
	float d2 = arcLengthTable[lower];

	return d1 * (1 - diff) + (diff)* d2;
}


vector<vector<float>> buildArcLengthCurveTable(TrainView *tv){
	vector<vector<float>> BigTable;
	float totalDistance = 0;
	Pnt3f lstPt;
	for (int i = 0; i < tv->distanceList.size(); i++){
		vector<float> smallTable;
		for (int u = 0; u <= 100; u++){
			float rU = u * 0.01;
			Pnt3f pt = getPointOnTrack(tv, rU, i);
			if (u == 0 && i == 0){
				totalDistance = 0;
			}
			else{
				totalDistance += (pt.distance(lstPt));
			}
			lstPt = pt;
			smallTable.push_back(totalDistance);
		}
		BigTable.push_back(smallTable);
	}
	return BigTable;
}


void Draw::drawTrack(TrainView *tv, bool doingShadow){

	std::vector<float> distanceList;
	int size = tv->world->points.size();
	//check which track to draw
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
		float increment = 0.01;
		
		for (int i = 0; i < tv->world->points.size(); i++){
			//get a point of the track
			float distance = 0;
			for (float j = 0; j < 1; j += increment){
				Pnt3f pt = getPointOnTrack(tv, j, i);
				Pnt3f dirPt = getDirectionVector(pt, tv, j, i);
				Pnt3f orPt = getOrientationVector(tv,j,i);

				//nextPoint
				Pnt3f nextPt = getPointOnTrack(tv, j + increment, i);

				if (j + increment < 1){
					distance += pt.distance(nextPt);
				}

				//push matrix
				glPushMatrix();
				//move to the correct point
				glTranslatef(pt.x, pt.y, pt.z);
				
				TrackModel::drawDual(pt, nextPt, orPt, doingShadow);
				
				glPopMatrix();				
			}

			distanceList.push_back(distance);
		}
	}
	
	//set distanceList
	tv->distanceList = distanceList;
	//set the arcLengthList
	tv->arcLengthTable = buildArcLengthCurveTableI(tv);
	 buildObjectAtInterval(tv, drawRailTrails, 10);
}

Pnt3f getNextPoint(TrainView *tv, float u){

	if ((u*100) > tv->arcLengthTable.size()){
		u -= (tv->arcLengthTable.size() * 0.01);
	}

	//get which line it is controlling
	int i = 0;
	while (u > 1){
		u -= 1;
		i++;
	}

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

vector<Pnt3f> getNextCarLocation(TrainView *tv, float& u, float distance){
	float oldD = getDistanceFromArcLength(tv,u);
	oldD -= distance;

	if (oldD < 0){
		oldD += tv->arcLengthTable[tv->arcLengthTable.size() - 1];
	}
	std::cout << "oldU:" << u << std::endl;
	std::cout << "oldD:" << oldD << std::endl;
	u = getPointFromArcLength(tv->arcLengthTable, oldD);
	std::cout << "newU:" << u << std::endl;

	Pnt3f pt = getNextPoint(tv, u);
	Pnt3f dirPt = getDirectionVector(pt, tv, u);
	Pnt3f orPt = getOrientationVector(tv, u);

	vector<Pnt3f> list;
	list.push_back(pt);
	list.push_back(orPt);
	list.push_back(dirPt);
	return list;
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

Pnt3f Draw::drawTrainN(TrainView *tv, bool doingShadow){

	drawBuildingModel(0, 0, 10);
	drawBuildingModel(100, 0, 20);
	drawBuildingModel(0, 100, 50);

	glPushMatrix();
	//get the u value
	float u = tv->world->trainU;
	//get the segment of the curve
	int i = tv->world->trainPoint;
	//get the total number of curve
	int size = tv->world->points.size();
	//get the point where the car should appear on the curves
	//uValidation(tv, u, i);
	std::cout << "u: " << u << std::endl;
	Pnt3f pt = getNextPoint(tv, u);
	//interpolate the orientation vector
	Pnt3f orPt = getOrientationVector(tv, u);
	//get a direcitonal vector 
	Pnt3f dirPt = getDirectionVector(pt, tv, u);

	//draw the front of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(pt.x, pt.y, pt.z);
	alignObjectIn3D(dirPt, orPt);
	//draw the front car
	TrainModel::drawFrontCar(doingShadow);
	glPopMatrix();

	//draw the middle cars
	int carNum = tv->tw->carNum->value();

	for (int j = 0; j < carNum; j++){
		//get a list of parameter for the next car

		vector<Pnt3f> list;
		list = getNextCarLocation(tv, u, 8);
		//draw the end of the train
		glPushMatrix();
		//move and rotate in the correct direction
		glTranslatef(list[0].x, list[0].y, list[0].z);
		alignObjectIn3D(list[2], list[1]);
		//draw the train model
		TrainModel::drawMiddleCar(doingShadow);
		glPopMatrix();
	}

	//get a list of parameter for the last car

	//vector<Pnt3f> list = getInformation(tv, u, i, size, pt, 7);
	vector<Pnt3f> list = getNextCarLocation(tv, u, 7);
	//draw the end of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(list[0].x, list[0].y, list[0].z);
	alignObjectIn3D(list[2], list[1]);

	//draw the train model
	TrainModel::drawBackCar(doingShadow);
	glPopMatrix();

	glPopMatrix();

	return dirPt;
}

Pnt3f Draw::drawTrain(TrainView *tv, bool doingShadow){


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
	TrainModel::drawFrontCar(doingShadow);
	glPopMatrix();
	
	//draw the middle cars
	int carNum = tv->tw->carNum->value();

	u = i + u;
	
	for (int j = 0; j < carNum; j++){
		//get a list of parameter for the next car
		
		vector<Pnt3f> list; 
		list = getNextCarLocation(tv, u, 8);
		//draw the end of the train
		glPushMatrix();
		//move and rotate in the correct direction
		glTranslatef(list[0].x, list[0].y, list[0].z);
		alignObjectIn3D(list[2], list[1]);
		//draw the train model
		TrainModel::drawMiddleCar(doingShadow);
		glPopMatrix();
	}

	//get a list of parameter for the last car
	
	//vector<Pnt3f> list = getInformation(tv, u, i, size, pt, 7);
	vector<Pnt3f> list = getNextCarLocation(tv, u, 7);
	//draw the end of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(list[0].x, list[0].y, list[0].z);
	alignObjectIn3D(list[2],list[1]);

	//draw the train model
	TrainModel::drawBackCar(doingShadow);
	glPopMatrix();

	glPopMatrix();

	return dirPt;
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

