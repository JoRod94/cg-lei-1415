#ifndef _SCALE_H
#define _SCALE_H
#include "transformation.h"

class Scale : public Transformation
{
public:
	float x, z, y;

	Scale(float x, float y, float z) : x(x), y(y), z(z) {};
	Scale() : x(0), y(0), z(0) {};

	virtual void apply();
};




#endif