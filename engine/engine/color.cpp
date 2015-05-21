#include "stdafx.h"
#include <GL/glut.h>
#include "color.h"
#include <iostream>

using namespace std;

void Color::apply(){
	if (material)
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cInfo);
	else
		glColor3ub(cInfo[0], cInfo[1], cInfo[2]);
}

