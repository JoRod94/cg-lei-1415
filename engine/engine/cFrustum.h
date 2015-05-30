#pragma once

#include "Vec3.h"
#include "plane.h"




class cFrustum {
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

	static enum { OUTSIDE, INTERSECT, INSIDE };

	plane pl[6];


	Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	cFrustum::cFrustum();
	cFrustum::~cFrustum();

	void cFrustum::setCamInternals(float angle, float ratio, float nearD, float farD);
	void cFrustum::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u);
	int cFrustum::pointInFrustum(Vec3 &p);
	int cFrustum::sphereInFrustum(Vec3 &p, float raio);

	void cFrustum::drawPoints();
	void cFrustum::drawLines();
	void cFrustum::drawPlanes();
	void cFrustum::drawNormals();

	bool gluInvertMatrix(const float m[16], float invOut[16]);
	Vec3 getWorldCoords(float m1[16], float m2[16]);
};
