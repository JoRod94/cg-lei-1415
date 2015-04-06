#include <GL/glut.h>
#include "transformation.cpp"

class Scale : public Transformation{
	void apply(float x, float y, float z){
		glScalef(x, y, z);
	}
};