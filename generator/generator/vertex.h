#pragma once
#include <point.h>
#include "vec3.h"

class vertex {
public:
	point p;
	vec3 n;

	vertex() : p(), n() {};
	vertex(point p, vec3 n) : p(p), n(n) {};
	vertex(const vertex &obj) = default;

	bool operator== (const vertex& v) const;
	bool operator< (const vertex& v) const;
	void rotate(float angle);
};