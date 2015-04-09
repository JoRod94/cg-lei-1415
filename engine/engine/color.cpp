#include "stdafx.h"
#include <GL/glut.h>
#include "color.h"


void Color::apply(){
	glColor3ub(r, g, b);
}

