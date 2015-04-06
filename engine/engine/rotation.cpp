#include <GL/glut.h>
#include "rotation.h"



void Rotation::apply(){
	glRotatef(angle, x, y, z);
}

