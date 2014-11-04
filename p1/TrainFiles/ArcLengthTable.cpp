#include "ArcLengthTable.h"


ArcLengthTable::ArcLengthTable()
{
}


ArcLengthTable::~ArcLengthTable()
{
}

void ArcLengthTable::setup(TrainView *tv){
	vector<vector<float>> arcTable;
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
}