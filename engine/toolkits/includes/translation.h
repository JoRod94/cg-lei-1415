#ifndef _TRANSLATE_H
#define _TRANSLATE_H
#include "transformation.h"
#include <iostream>
#include "point.h"

using namespace std;


#pragma
class Translation : public Transformation
{
public:
	float x, z, y;
	long time;
	float up[3];
	vector <point> points;
	float t = 0.0f;

	Translation(long time, float x, float y, float z) : time(time), x(x), y(y), z(z) { up[0] = 0; up[1] = 1; up[2] = 0; };
	Translation(float x, float y, float z) : time(0), x(x), y(y), z(z) { up[0] = 0; up[1] = 1; up[2] = 0; };
	Translation(long time) : time(time), x(0), y(0), z(0) { up[0] = 0; up[1] = 1; up[2] = 0; };
	Translation() : time(0), x(0), y(0), z(0) { up[0] = 0; up[1] = 1; up[2] = 0; };

	virtual void apply();
};


#endif