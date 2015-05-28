#include "color.h"
#include <GL/glut.h>
#include <string>

#pragma once
class Model
{
public:
	Color* color;
	unsigned int texId;
	std::string filename;

	Model();
	Model(Color* color, unsigned int texId, std::string filename) :
		color(color), texId(texId), filename(filename) {};
	~Model();

	void Model::draw();
};
