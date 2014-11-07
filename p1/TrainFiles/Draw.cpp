//please read Draw.h for instructions and usage
//xiang zhi tan 2014

#include "Draw.h"

/*
 * Cardinal Curve Calculation
 */

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

/* deconstruct U into which line it is on */
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

/* get orientation vector for u*/
Pnt3f getOrientationVector(TrainView *tv, float u){
	int i;
	uValidation(tv, u, i);
	int size = tv->world->points.size();
	return (1 - u) * tv->world->points[i].orient + u * tv->world->points[(i + 1) % size].orient;
}

/* get directional vector*/
Pnt3f getDirectionVector(Pnt3f pt, TrainView *tv, float u){
	//get the next small point on the line
	Pnt3f pt2 = getPointOnTrack(tv, u + 0.01);
	//get a direcitonal vector from the two points;
	return pt2 - pt;
}

typedef void(*drawCallback)(Pnt3f pt, Pnt3f dirPt, Pnt3f orPt);

/* build the arc Length table*/
vector<float> buildArcLengthCurveTable(TrainView *tv){
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
	return arcTable;
}

/* draw object at a certain interval on the track*/
void buildObjectAtInterval(TrainView *tv, drawCallback callback, float interval){

	vector<float> arcTable = tv->arcLengthTable;

	float curPoint = 0;
	float nextPoint = 0;
	float total = arcTable[arcTable.size() - 1];
	float index = 0;
	while (nextPoint < total && index < arcTable.size()){
		while (nextPoint < arcTable[index]){

			float u = index * 0.01;

			Pnt3f pt = getPointOnTrack(tv, u);
			Pnt3f dirPt = getDirectionVector(pt, tv, u);
			Pnt3f orPt = getOrientationVector(tv, u);

			//call the drawing callback
			callback(pt,dirPt,orPt);
			nextPoint += interval;
		}
		index++;
	}
}
/* get a point from the arc length table given*/
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

/* get the distance of a point from arc length table*/
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

	return d2 * (1 - diff) + (diff)* d1;
}

/* draw the track*/
void Draw::drawTrack(TrainView *tv, bool doingShadow){
	TrackModel::clearIt();
	std::vector<float> distanceList;
	int size = tv->world->points.size();
	//check which track to draw
	if (tv->tw->splineBrowser->value() == 1){
		for (int i = 0; i < tv->world->points.size(); i++){

			Pnt3f orPt(0, 1, 0);
			Pnt3f pt = tv->world->points[(i) % size].pos;

			if (tv->tw->trackBrowser->value() == 1){
				TrackModel::drawSimple(tv->world->points[(i) % size].pos, 
					tv->world->points[(i + 1) % size].pos, orPt, doingShadow);
			}
			else if (tv->tw->trackBrowser->value() == 2){
				TrackModel::drawDual(tv->world->points[(i) % size].pos,
					tv->world->points[(i + 1) % size].pos, orPt, doingShadow);
			}

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
			for (float j = 0; j < 1; j += increment	){
				float u = i + j;
				Pnt3f pt = getPointOnTrack(tv, u);
				Pnt3f dirPt = getDirectionVector(pt, tv, u);
				Pnt3f orPt = getOrientationVector(tv, u);

				//nextPoint
				Pnt3f nextPt = getPointOnTrack(tv, j + increment, i);

				if (j + increment < 1){
					distance += pt.distance(nextPt);
				}
				
				//select which type of track to draw
				if(tv->tw->trackBrowser->value() == 1){
					TrackModel::drawSimple(pt, nextPt, orPt, doingShadow);
				}
				else if (tv->tw->trackBrowser->value() == 2){
					TrackModel::drawDual(pt, nextPt, orPt, doingShadow);
				}		
			}
			distanceList.push_back(distance);
		}
		
	}
	//draw the track
	TrackModel::drawIt();
	//set distanceList
	tv->distanceList = distanceList;
	//set the arcLengthList
	tv->arcLengthTable = buildArcLengthCurveTable(tv);
	if (tv->tw->ties->value() == 1){
		buildObjectAtInterval(tv, TrackModel::drawRailTrails, 10);
	}
	
}

/* return the point where u is on the track*/
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

/* get where the next car should be*/
vector<Pnt3f> getNextCarLocation(TrainView *tv, float& u, float distance){

	if (tv->tw->arcLength->value()){
		float oldD = getDistanceFromArcLength(tv, u);
		oldD -= distance;

		if (oldD < 0){
			oldD += tv->arcLengthTable[tv->arcLengthTable.size() - 1];
		}
		u = getPointFromArcLength(tv->arcLengthTable, oldD);
	}
	else{
		u -= distance * 0.01;
	}

	Pnt3f pt = getNextPoint(tv, u);
	Pnt3f dirPt = getDirectionVector(pt, tv, u);
	Pnt3f orPt = getOrientationVector(tv, u);

	vector<Pnt3f> list;
	list.push_back(pt);
	list.push_back(orPt);
	list.push_back(dirPt);
	return list;
}

/* draw the train*/
vector<Pnt3f> Draw::drawTrain(TrainView *tv, bool doingShadow){

	glPushMatrix();
	//get the u value
	float u = tv->world->trainU;
	//get the segment of the curve
	int i = tv->world->trainPoint;
	//get the total number of curve
	int size = tv->world->points.size();
	//get the point where the car should appear on the curves
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
	vector<Pnt3f> list = getNextCarLocation(tv, u, 7);
	//draw the end of the car
	glPushMatrix();
	//move and rotate in the correct direction
	glTranslatef(list[0].x, list[0].y, list[0].z);
	std::cout << list[0] << pt;
	alignObjectIn3D(list[2], list[1]);

	//draw the train model
	TrainModel::drawBackCar(doingShadow);
	glPopMatrix();

	glPopMatrix();

	list.clear();
	list.push_back(pt);
	list.push_back(dirPt);
	return list;
}

/* return information for where the train is looking at*/
vector<Pnt3f> Draw::getLookingPoints(TrainView *tv){
	float u = tv->world->trainU;
	int size = tv->world->points.size();
	Pnt3f eyeVector = getNextPoint(tv, u);
	//where to look next
	Pnt3f pt2 = getNextPoint(tv, u + 0.01);
	//where is up
	Pnt3f upVector = getOrientationVector(tv, u);
	Pnt3f dirPt = getDirectionVector(eyeVector, tv, u);
	vector<Pnt3f> list;
	//where the eye is
	list.push_back(eyeVector);
	//where to look at
	list.push_back(pt2);
	//where is up
	list.push_back(upVector);
	//what is the directional vector
	list.push_back(dirPt);
	return list;
}

