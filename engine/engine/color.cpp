#include "stdafx.h"
#include <GL/glut.h>
#include "color.h"
#include "engine.h"
#include <iostream>

using namespace std;

const float Color::reset[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float Color::white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const float Color::green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

void Color::apply(){
	if (glIsEnabled(GL_LIGHTING) || material) {
		glMaterialfv(GL_FRONT, color_type, cInfo);
	}
	else {
		glColor3ub(cInfo[0], cInfo[1], cInfo[2]);
	}

	change_color();
}

void Color::reset_type() {
	if ((glIsEnabled(GL_LIGHTING) || material) && color_type != GL_AMBIENT_AND_DIFFUSE)
		glMaterialfv(GL_FRONT, color_type, Color::reset);
}

