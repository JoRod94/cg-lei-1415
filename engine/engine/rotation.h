#ifndef _ROTATE_H
#define _ROTATE_H
#include "transformation.cpp"

class Rotation : public Transformation
{
public:
	float x, z, y;
	float angle;

	Rotation(float angle, float x, float y, float z) : angle(angle), x(x), y(y), z(z) {};
	Rotation() : angle(0), x(0), y(0), z(0) {};

	void apply();

};

#endif