#pragma once
#include <point.h>
#include "vec3.h"

class vertex {
public:
	point p;
	vec3 n;
	float tc_x, tc_y;

	vertex() : p(), n(), tc_x(0), tc_y(0) {};
	vertex(point p, vec3 n) : p(p), n(n), tc_x(0), tc_y(0) {};
	vertex(point p, vec3 n, float tcx, float tcy) : p(p), n(n), tc_x(tcx), tc_y(tcy) {};
	vertex(const vertex &obj) = default;

	bool operator== (const vertex& v) const;
	bool operator< (const vertex& v) const;
	void put_tc(float tcx, float tcy);
	void rotate(float angle);
};