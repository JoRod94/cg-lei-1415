#ifndef _COLOR_H
#define _COLOR_H
#include "transformation.h"

class Color : public Transformation {
public:
	float cInfo[4];
	bool material;

	Color(float r, float g, float b) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = false;
	}

	Color(float r, float g, float b, bool m) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = m;
	}

	Color() {
		cInfo[0] = 0;
		cInfo[1] = 0;
		cInfo[2] = 0;
		cInfo[3] = 1.0f;
		material = false;
	}

	virtual void apply();
};

#endif
