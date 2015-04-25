#include "stdafx.h"
#include <GL/glut.h>
#include "translation.h"


void Translation::apply(){
	// TODO: Update this to the timed version
	glTranslatef(x, y, z);
}

