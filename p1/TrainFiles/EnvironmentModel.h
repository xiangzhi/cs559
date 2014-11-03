#pragma once

#include <vector>
#include  "ModelInfo.h"
#include "BuildingModel.h"

class EnvironmentModel
{
public:
	EnvironmentModel();
	~EnvironmentModel();
	void draw();
	void reset();

	//variables
	std::vector<ModelInfo> modelList;
};

