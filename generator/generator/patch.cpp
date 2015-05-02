#include "stdafx.h"
#include "patch.h"


Patch::Patch()
{
}


Patch::~Patch()
{
}

point Patch::getPoint(int i) {
	return this->points[(this->indices[ci])[i]];
}

void Patch::next_patch() {
	ci = (ci + 1) % this->indices.size();
}
