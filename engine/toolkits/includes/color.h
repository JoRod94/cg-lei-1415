#ifndef _COLOR_H
#define _COLOR_H
#include "transformation.h"
#include <GL/glut.h>

class Color : public Transformation {
public:
	float cInfo[4];
	GLenum color_type;
	bool material;
	static const float reset[4];
	static const float white[4];
	static const float green[4];

	Color(float r, float g, float b) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = false;
		color_type = GL_AMBIENT_AND_DIFFUSE;
	}

	Color(float r, float g, float b, bool m) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = m;
		color_type = GL_AMBIENT_AND_DIFFUSE;
	}
	
	Color(float r, float g, float b, GLenum type, bool m) {
		cInfo[0] = r;
		cInfo[1] = g;
		cInfo[2] = b;
		cInfo[3] = 1.0f;
		material = m;
		color_type = type;
	}

	Color() {
		cInfo[0] = 0;
		cInfo[1] = 0;
		cInfo[2] = 0;
		cInfo[3] = 1.0f;
		material = false;
		color_type = GL_AMBIENT_AND_DIFFUSE;
	}

	virtual void apply();
	void reset_type();
};

#endif
