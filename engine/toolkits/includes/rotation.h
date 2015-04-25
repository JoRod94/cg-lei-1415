#ifndef _ROTATE_H
#define _ROTATE_H
#include "transformation.h"

#pragma
class Rotation : public Transformation
{
public:
	float x, z, y;
	float angle;
	long time;

	Rotation(float angle, long time, float x, float y, float z) : angle(angle), time(time), x(x), y(y), z(z) {};
	Rotation(float angle, float x, float y, float z) : angle(angle), x(x), y(y), z(z), time(0) {};
	Rotation(long time, float x, float y, float z) : time(time), x(x), y(y), z(z), angle(0) {};
	Rotation() : angle(0), time(0), x(0), y(0), z(0) {};

	virtual void apply();

};

#endif