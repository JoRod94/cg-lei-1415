#include "stdafx.h"
#include <GL/glut.h>
#include "color.h"
#include "engine.h"
#include <iostream>

using namespace std;

void Color::apply(){
	if (glIsEnabled(GL_LIGHTING) || material) {
		glMaterialfv(GL_FRONT, color_type, cInfo);
	}
	else {
		glColor3ub(cInfo[0], cInfo[1], cInfo[2]);
	}

	change_color();
}

