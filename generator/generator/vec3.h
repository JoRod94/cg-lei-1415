#pragma once

class vec3 {
public:
	float x, y, z;

	vec3() : x(0), y(0), z(0) {};
	vec3(float x, float y, float z) : x(x), y(y), z(z) {};
	vec3(const vec3 &obj) = default;

	bool operator== (const vec3& v) const;
	bool operator< (const vec3& v) const;
	vec3 operator+ (const vec3& v) const;
	vec3 operator- (const vec3& v) const;
	vec3 operator* (const vec3& v) const;
	vec3 operator+ (const float& n) const;
	vec3 operator- (const float& n) const;
	vec3 operator* (const float& n) const;
	vec3 operator/ (const float& n) const;

	float length();
	void normalize();
	float dot(vec3 v);
	vec3 cross(vec3 v);
};