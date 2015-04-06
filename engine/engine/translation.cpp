#include <GL/glut.h>
#include "transformation.cpp"

class Translation : public Transformation{
	void apply(float x, float y, float z){
		glTranslatef(x, y, z);
	}
};