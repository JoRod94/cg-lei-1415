#include <GL/glut.h>
#include "scale.h"



void Scale::apply(){
	glScalef(x, y, z);
}

