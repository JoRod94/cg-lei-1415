#include "stdafx.h"
#include "vertex.h"

bool vertex::operator==(const vertex& v) const{
	return ((p == v.p) && (n == v.n));
}

bool vertex::operator< (const vertex& v) const {
	return p < v.p
		|| (p == v.p && n < v.n);
}

void vertex::put_tc(float tcx, float tcy){
	tc_x = tcx;
	tc_y = tcy;
}

void vertex::rotate(float angle){
	p.x = p.x * sin(angle);
	p.z = p.z * cos(angle);
	n.x = n.x * sin(angle);
	n.z = n.z * cos(angle);
}