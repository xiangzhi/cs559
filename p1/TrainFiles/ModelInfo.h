
#include <vector>
#include "Utilities\Pnt3f.h"


#pragma once
class ModelInfo
{
public:
	ModelInfo();
	~ModelInfo();
	Pnt3f pos;
	Pnt3f orientation;
	Pnt3f direction;
	float height = -1;
	float width = -1;
	float length = -1;
};

