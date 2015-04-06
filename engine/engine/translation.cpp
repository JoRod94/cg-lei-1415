#include <GL/glut.h>
#include "translation.h"



void Translation::apply(){
	glTranslatef(x, y, z);
}

