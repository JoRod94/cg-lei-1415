#ifndef _COLOR_H
#define _COLOR_H
#include "transformation.h"
#include <GL/glut.h>

class Color : public Transformation {
public:
	float cInfo[4];
	GLenum c_type;
	bool material;

	Color(float r, float g, float b) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = false;
		c_type = GL_AMBIENT_AND_DIFFUSE;
	}

	Color(float r, float g, float b, bool m) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = m;
		c_type = GL_AMBIENT_AND_DIFFUSE;
	}
	
	Color(float r, float g, float b, GLenum type, bool m) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = m;
		c_type = type;
	}

	Color() {
		cInfo[0] = 0;
		cInfo[1] = 0;
		cInfo[2] = 0;
		cInfo[3] = 1.0f;
		material = false;
		c_type = GL_AMBIENT_AND_DIFFUSE;
	}

	virtual void apply();
};

#endif
