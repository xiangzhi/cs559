#include "EnvironmentModel.h"


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
		drawBuildingModel(modelList[i].pos.x, modelList[i].pos.z, modelList[i].height);
	}
}


void EnvironmentModel::reset(){
	
	modelList.clear();

	for (int i = 0; i < 10; i++){
		ModelInfo info;
		info.height = (rand() % 50) + 30;
		info.pos.x = (rand() % 200) - 100;
		info.pos.z = (rand() % 200) - 100;
		
		modelList.push_back(info);
	}
}