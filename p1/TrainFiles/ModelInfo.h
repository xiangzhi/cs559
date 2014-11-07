
#include <vector>
#include "Utilities\Pnt3f.h"

/* used to store information about something to draw*/

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

