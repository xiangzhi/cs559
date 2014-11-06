#pragma once

#include <vector>
#include  "ModelInfo.h"
#include "BuildingModel.h"
#include "Utilities\3DUtils.h"

class EnvironmentModel
{
public:
	EnvironmentModel();
	~EnvironmentModel();
	static void draw();
	static void reset();
	static void drawFloor(int x);
	static void drawEnvironment(bool shadow = false);
	//variables
	static std::vector<ModelInfo> modelList;
	static std::vector<std::vector<Pnt3f>> waveList;
	static std::vector<Pnt3f> waveListGraph;
	static int counter;
};

