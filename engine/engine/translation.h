#ifndef _TRANSLATE_H
#define _TRANSLATE_H
#include "transformation.cpp"



class Translation : public Transformation
{
public:
	float x, z, y;

	Translation(float x, float y, float z) : x(x), y(y), z(z) {};
	Translation() : x(0), y(0), z(0) {};

	void apply();
};


#endif