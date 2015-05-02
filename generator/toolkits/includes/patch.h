#pragma once
#include <vector>
#include "point.h"
class Patch
{
public:
	std::vector<point> points;
	std::vector< vector<int> > indices;
	Patch();
	~Patch();
	point getPoint(int i);
	void next_patch();
private:
	int ci;
};

