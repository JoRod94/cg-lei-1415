#include "stdafx.h"
#include <GL/glut.h>
#include "rotation.h"



void Rotation::apply(){
	// TODO: Update this to the timed version
	glRotatef(angle, x, y, z);
}

