#include <GL/glut.h>
#include "transformation.cpp"

class Rotation : public Transformation{
	void apply(double degrees, double x, double y, double z){
		glRotatef(degrees, x, y, z);
	}
};