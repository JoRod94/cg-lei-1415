#include "stdafx.h"
#include "point.h"


#include <vector>

using namespace std;

bool point::operator== (const point& p) const {
	return x == p.x && y == p.y && z == p.z;
}

void point::add_to(vector<float> &vec) {
	vec.push_back(x);
	vec.push_back(y);
	vec.push_back(z);
}

void point::move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

void point::reset() {
	x = 0;
	y = 0;
	z = 0;
}
