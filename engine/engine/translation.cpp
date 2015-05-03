#include "stdafx.h"
#include <GL/glut.h>
#include "translation.h"
#include <iostream>


void getCatmullRomPoint(float t, int *indices, float *res, float *der, vector<point> p) {

	// catmull-rom matrix
	float m[4][4] = { { -0.5f, 1.5f, -1.5f, 0.5f },
	{ 1.0f, -2.5f, 2.0f, -0.5f },
	{ -0.5f, 0.0f, 0.5f, 0.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f } };

	res[0] = 0.0; res[1] = 0.0; res[2] = 0.0;
	der[0] = 0.0; der[1] = 0.0; der[2] = 0.0;
	float temp[4];
	float temp2[4];



	float c;
	for (int j = 0; j < 4; j++){
		c = 0;
		for (int i = 0; i < 4; i++){
			c += pow(t, (3 - i)) * m[i][j];
		}
		temp[j] = c;
		temp2[j] = 3 * pow(t, 2) * m[0][j] + 2 * t * m[1][j] + m[2][j];
	}

	res[0] = temp[0] * p[indices[0]].x + temp[1] * p[indices[1]].x + temp[2] * p[indices[2]].x + temp[3] * p[indices[3]].x;
	res[1] = temp[0] * p[indices[0]].y + temp[1] * p[indices[1]].y + temp[2] * p[indices[2]].y + temp[3] * p[indices[3]].y;
	res[1] = temp[0] * p[indices[0]].z + temp[1] * p[indices[1]].z + temp[2] * p[indices[2]].z + temp[3] * p[indices[3]].z;

}


// given  global t, returns the point in the curve
void getGlobalCatmullRomPoint(float gt, float *res, float *der, vector<point> p) {
	int pointCount = p.size();

	float t = gt * pointCount; // this is the real global t
	int index = floor(t);  // which segment
	t = t - index; // where within  the segment

	// indices store the points
	int indices[4];
	indices[0] = (index + pointCount - 1) % pointCount;	indices[1] = (indices[0] + 1) % pointCount;
	indices[2] = (indices[1] + 1) % pointCount; indices[3] = (indices[2] + 1) % pointCount;


	getCatmullRomPoint(t, indices, res, der, p);
}

void renderCatmullRomCurve(float *res, float *der, int pointCount, vector<point> p) {
	float gt = 0.0f;
	glBegin(GL_LINE_LOOP);
	while (gt < 1.0f){
		getGlobalCatmullRomPoint(gt, res, der, p);
		glVertex3f(res[0], res[1], res[2]);
		gt += 0.001f;
	}
	glEnd();
}

void normalize(float* v, int n) {
	float norm = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	for (int i = 0; i < n; i++)
		v[i] /= norm;
}

float * normalizedCrossProduct(float *a, float *b){
	float result[3];
	result[0] = a[1] * b[2] - b[1] * a[2];
	result[1] = a[2] * b[0] - b[2] * a[0];
	result[2] = a[0] * b[1] - b[0] * a[1];
	normalize(result, 3);
	return result;
}

void curveRotation(float *der, float *p, float *up){

	float *left;
	
	normalize(der, 3);
	left = normalizedCrossProduct(up, der);
	up = normalizedCrossProduct(der, left);

	float m[16] = { left[0], left[1], left[2], 0, up[0], up[1], up[2], 0, der[0], der[1], der[2], 0, p[0], p[1], p[2], 1 };
	glMultMatrixf(m);
}

void Translation::apply(){
	//Normal Translation
	if(time == 0)
		glTranslatef(x, y, z);
	else{ //Curved and Timed Translation
		float res[3];
		float der[3];

		renderCatmullRomCurve(res, der, points.size(), points);

		getGlobalCatmullRomPoint(t, res, der, points);
		glTranslatef(res[0], res[1], res[2]);
		float p[3] = { 0, 0, 0 };
		curveRotation(der,p, up);

		t = fmod(glutGet(GLUT_ELAPSED_TIME) / (time*1000.0f), 1);
	}
	
	

}

