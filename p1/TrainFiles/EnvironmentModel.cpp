#include "EnvironmentModel.h"

std::vector<std::vector<Pnt3f>> EnvironmentModel::waveList;
std::vector<Pnt3f> EnvironmentModel::waveListGraph;
std::vector<ModelInfo> EnvironmentModel::modelList;
int EnvironmentModel::counter = 0;

EnvironmentModel::EnvironmentModel()
{
}


EnvironmentModel::~EnvironmentModel()
{
}


void EnvironmentModel::draw(){
	if (modelList.size() == 0){
		reset();
	}
	
	for (int i = 0; i < modelList.size(); i++){
		glPushMatrix();
		glRotatef(modelList[i].orientation.x, 1, 0, 0);
		glRotatef(modelList[i].orientation.y, 0, 1, 0);
		drawBuildingModel(modelList[i].pos.x, modelList[i].pos.z, modelList[i].height);
		glPopMatrix();
	}
	
}

Pnt3f calBezierQuadPointsN(float u, Pnt3f p1, Pnt3f p2, Pnt3f p3){
	return ((p1 * ((1 - u) * (1 - u))) + (2 * (1 - u) * u * p2) + ((u * u) * p3));
}

std::vector<Pnt3f> addWave(){
	float distance = -110;
	float height = (rand()%10) + 2;
	std::vector<Pnt3f> list;
	list.push_back(Pnt3f(distance, 0, -100));
	list.push_back(Pnt3f(distance + 15, height, -100));
	list.push_back(Pnt3f(distance + 30, 0, -100));
	list.push_back(Pnt3f(0, 0, 0));
	return list;
}

std::vector<Pnt3f> createWave(int start){
	float distance = 0;
	float height = (rand() % 6) + 1;
	std::vector<Pnt3f> list;
	for (int i = 0; distance < 300;i++){
		int y = 0;
		if (i % 2 == 1){
			y = height;
		}
		if (i % 4 == 0){
			y = -1 * y;
		}
		list.push_back(Pnt3f(distance, y, -100));
		distance += (10) + (rand()%5);
	}	
	list.push_back(Pnt3f(300 + 1, 0, -100));
	list.push_back(Pnt3f(start, 0, 0));
	return list;
}

float addX(int x, int increase){
	x += increase;
	if (x < -100){
		x = 100;
	}
	if (x > 100){
		x = -100;
	}
	return x;
}

void EnvironmentModel::drawEnvironment(bool shadow){
	if (modelList.size() == 0){
		reset();
	}
	for (int j = 0; j < 4; j++){
		glPushMatrix();
		glRotatef(90 * j, 0, 1, 0);
		for (int i = 0; i < modelList.size(); i++){
			glPushMatrix();

			glTranslatef(modelList[i].pos.x, 0, modelList[i].pos.z);

			glRotatef(modelList[i].orientation.z, 0, 0, 1);
			glRotatef(modelList[i].orientation.y, 0, 1, 0);
			drawBuildingModel(modelList[i].pos.x, modelList[i].pos.z, modelList[i].height);
			glPopMatrix();
		}
		glPopMatrix();
	}

	glPushMatrix();
	if (!shadow){
		glColor3ub(139, 69, 19);
	}
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(quadratic, 5, 3, 50, 32, 32);
	glPopMatrix();

	glPushMatrix();

	if (!shadow){
		glColor3ub(255,0,0);
	}
	glTranslatef(0, 50, 0);
	drawCube(0, 0, 0, 7);
	glPopMatrix();

}


void EnvironmentModel::drawFloor(int x){
	

	GLdouble arr0[] = { 0, 40000, 0, 0 };
	glClipPlane(GL_CLIP_PLANE3, arr0);
	glEnable(GL_CLIP_PLANE3);
	/*
	glColor3ub(0, 0, 255);
	
	glBegin(GL_QUADS);
	glVertex3f(-100,0 , -100);
	glVertex3f(100,	0, -100);
	glVertex3f(100,	0, 100);
	glVertex3f(-100,0, 100);
	glEnd();
	*/



	glPushMatrix();
	
	GLdouble arr[] = { -20000, 0, 0, 2000000 };
	GLdouble arr2[] = { -20000, 0, 0, -2000000 };
	glClipPlane(GL_CLIP_PLANE0, arr);
	glClipPlane(GL_CLIP_PLANE1, arr);
	glEnable(GL_CLIP_PLANE0);
	glEnable(GL_CLIP_PLANE1);

	int add = 5;
	//glTranslatef(x + 0, 0, 0);

	glColor3ub(0, 0, 255);
	if (waveList.size() == 0){
		waveList.push_back(createWave(-400));
		waveList.push_back(createWave(-200));
		waveList.push_back(createWave(100));
	}

	for (int k = 0; k < waveList.size(); k++){
		std::vector<Pnt3f> wave = waveList[k];

		glPushMatrix();

		Pnt3f t = wave[wave.size() - 1];

		if (x != wave[wave.size() - 1].y){
			waveList[k][wave.size() - 1].y = x;
			waveList[k][wave.size() - 1].x++;
		}

		//rotate the wave plane
		if (waveList[k][wave.size() - 1].x > 300){

			waveList[k] = waveList[(k - 1 + waveList.size()) % waveList.size()];
			waveList[(k - 2 + waveList.size()) % waveList.size()] = waveList[(k - 2 + waveList.size()) % waveList.size()];
			waveList[(k - 3 + waveList.size()) % waveList.size()] = createWave(-400);
		}

		glTranslatef(t.x, 3, 0);
		for (int j = 0; j < (wave.size() - 3); j += 2){
			Pnt3f p1 = wave[j];;
			Pnt3f p2 = wave[j + 1];
			Pnt3f p3 = wave[j + 2];
			for (float i = 0; i <= 1; i += 0.05){
				glBegin(GL_QUADS);
				Pnt3f pt = (calBezierQuadPointsN(i, p1, p2, p3));
				glVertex3f(pt.x, pt.y, pt.z);
				pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
				glVertex3f(pt.x, pt.y, pt.z);
				pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
				glVertex3f(pt.x, pt.y, pt.z * -1);
				pt = (calBezierQuadPointsN(i, p1, p2, p3));
				glVertex3f(pt.x, pt.y, pt.z * -1);
				glEnd();
			}
		}
		glPopMatrix();


	}
	/*
	glPushMatrix();

	Pnt3f t = wave[wave.size() -1];

	if (x != wave[wave.size() - 1].y){
		waveListGraph[wave.size() - 1].y = x;
		waveListGraph[wave.size() - 1].x++;
	}

	glTranslatef(t.x, 3, 0);
	for (int j = 0; j < (wave.size() - 3);  j += 2){
		Pnt3f p1 = wave[j];;
		Pnt3f p2 = wave[j + 1];
		Pnt3f p3 = wave[j + 2];
		for (float i = 0; i <= 1; i += 0.05){
			glBegin(GL_QUADS);
			Pnt3f pt = (calBezierQuadPointsN(i, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z);
			pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z);
			pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z * -1);
			pt = (calBezierQuadPointsN(i, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z * -1);
			glEnd();
		}
	}
	glPopMatrix();
	/*
	if (x % 35 == 0){
		waveList.push_back(addWave());
	}

	if (waveList.size() == 0){
		waveList.push_back(addWave());
	}

	for (int j = 0; j < waveList.size(); j++){
		Pnt3f p1 = waveList[j][0];
		Pnt3f p2 = waveList[j][1];
		Pnt3f p3 = waveList[j][2];
		Pnt3f t = waveList[j][3];
		if (t.x > 200){
			waveList.erase(waveList.begin() + j);
		}

		glPushMatrix();
		glTranslatef(t.x, 0, 0);
		if (x != waveList[j][3].y){
			waveList[j][3].y = x;
			waveList[j][3].x++;
		}
		for (float i = 0; i <= 1; i += 0.05){
			glBegin(GL_QUADS);
			Pnt3f pt = (calBezierQuadPointsN(i, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z);
			pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z);
			pt = (calBezierQuadPointsN(i + 0.05, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z * -1);
			pt = (calBezierQuadPointsN(i, p1, p2, p3));
			glVertex3f(pt.x, pt.y, pt.z * -1);
			glEnd();
		}

		glPopMatrix();
	}
	*/

	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE3);
	glPopMatrix();
}


void EnvironmentModel::reset(){
	
	modelList.clear();

	for (int i = 0; i < 6; i++){
		ModelInfo info;
		info.height = (rand() % 30) + 20;
		info.pos.x = (i * 30) - 100;
		info.pos.z = -90 + (rand()%30);
		info.orientation.x = 1;
		info.orientation.y = -90;
		info.orientation.z = (rand() % 90);
		if (rand() % 2 == 0){
			info.orientation.z *= -1;
		}
		modelList.push_back(info);
	}
	/*
	int size = modelList.size();
	for (int i = size; i < size + 6; i++){
		ModelInfo info;
		info.height = (rand() % 30) + 20;
		info.pos.x = -90 + (rand() % 30);
		info.pos.z = (i * 30) - 100;
		info.orientation.x = 1;
		info.orientation.y = -90;
		info.orientation.z = (rand() % 90);
		if (rand() % 2 == 0){
			info.orientation.z *= -1;
		}
		modelList.push_back(info);
	}*/
}