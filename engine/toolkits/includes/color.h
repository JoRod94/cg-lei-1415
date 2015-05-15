#ifndef _COLOR_H
#define _COLOR_H
#include "transformation.h"

class Color : public Transformation
{
public:
	float r, g, b;

	Color(float r, float g, float b) : r(r), g(g), b(b) {};
	Color() : r(0), g(0), b(0) {};

	virtual void apply();
};

#endif
