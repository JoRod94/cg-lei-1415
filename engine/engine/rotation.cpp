#include "stdafx.h"
#include <GL/glut.h>
#include "rotation.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

using namespace std;



void Rotation::apply(){
	if (time == 0)
		glRotatef(angle, x, y, z);
	else{
		float ang = fmod((glutGet(GLUT_ELAPSED_TIME)*360.0f ) / (time*1000.0f), 360.0f);
		glRotatef(ang, x, y, z);
	}
}

