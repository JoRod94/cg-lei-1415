#pragma once

#include "Vec3.h"

class Vec3;

class plane {

public:

	Vec3 normal, point;
	float d;


	plane::plane(Vec3 &v1, Vec3 &v2, Vec3 &v3);
	plane::plane(void);
	plane::~plane();

	void set3Points(Vec3 &v1, Vec3 &v2, Vec3 &v3);
	void setNormalAndPoint(Vec3 &normal, Vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(Vec3 &p);
};