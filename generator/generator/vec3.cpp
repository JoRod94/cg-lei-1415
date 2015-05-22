#include "stdafx.h"
#include "vec3.h"
#include <math.h>

bool vec3::operator==(const vec3& v) const {
	return ((x == v.x) && (y == v.y) && (z == v.z)) ? true : false;
}

bool vec3::operator< (const vec3& v) const {
	return x < v.x
		|| (x == v.x && y < v.y)
		|| (x == v.x && y == v.y && z < v.z);
}

vec3 vec3::operator+(const vec3& v) const {
	return vec3((x + v.x), (y + v.y), (z + v.z));
}

vec3 vec3::operator-(const vec3& v) const {
	return vec3((x - v.x), (y - v.y), (z - v.z));
}

vec3 vec3::operator* (const vec3& v) const {
	return vec3((x * v.x), (y * v.y), (z * v.z));
}

vec3 vec3::operator+(const float& n) const {
	return vec3((x + n), (y + n), (z + n));
}

vec3 vec3::operator-(const float& n) const {
	return vec3((x - n), (y - n), (z - n));
}

vec3 vec3::operator*(const float& n) const {
	return vec3((x * n), (y * n), (z * n));
}

vec3 vec3::operator/(const float& n) const {
	//checks if we ared dividing by 0
	return (n) ? vec3((x / n), (y / n), (z / n)) : *this;
}

float vec3::length() {
	return sqrt(x*x + y*y + z*z);
}

void vec3::normalize() {
	*this / (this->length());
}

float vec3::dot(vec3 v) {
	return (x * v.x) + (y * v.y) + (z * v.z);
}

vec3 vec3::cross(vec3 v) {
	return vec3(
		(y*v.z) - (z*v.y),
		(z*v.x) - (x*v.z),
		(x*v.y) - (y*v.x)
		);
}
