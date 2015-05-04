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



	float c, c2;
	for (int j = 0; j < 4; j++){
		c = 0;
		c2 = 0;
		for (int i = 0; i < 4; i++){
			c += pow(t, (3 - i)) * m[i][j];
			if (i < 3) c2 += (3 - i)*pow(t, (2 - i))*m[i][j];
		}
		temp[j] = c;
		temp2[j] = c2;
	}

	res[0] = temp[0] * p[indices[0]].x + temp[1] * p[indices[1]].x + temp[2] * p[indices[2]].x + temp[3] * p[indices[3]].x;
	res[1] = temp[0] * p[indices[0]].y + temp[1] * p[indices[1]].y + temp[2] * p[indices[2]].y + temp[3] * p[indices[3]].y;
	res[2] = temp[0] * p[indices[0]].z + temp[1] * p[indices[1]].z + temp[2] * p[indices[2]].z + temp[3] * p[indices[3]].z;

	der[0] = temp2[0] * p[indices[0]].x + temp2[1] * p[indices[1]].x + temp2[2] * p[indices[2]].x + temp2[3] * p[indices[3]].x;
	der[1] = temp2[0] * p[indices[0]].y + temp2[1] * p[indices[1]].y + temp2[2] * p[indices[2]].y + temp2[3] * p[indices[3]].y;
	der[2] = temp2[0] * p[indices[0]].z + temp2[1] * p[indices[1]].z + temp2[2] * p[indices[2]].z + temp2[3] * p[indices[3]].z;

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
	glColor3f(1, 1, 1);
	float gt = 0.0f;
	glBegin(GL_LINE_LOOP);
	while (gt < 1.0f){
		getGlobalCatmullRomPoint(gt, res, der, p);
		glVertex3f(res[0], res[1], res[2]);
		gt += 0.0001f;
	}
	glEnd();
}

void normalize(float* v, int n) {
	float norm = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	for (int i = 0; i < n; i++)
		v[i] /= norm;
}

void crossProduct(float *r, float *a, float *b){
	
	r[0] = a[1] * b[2] - b[1] * a[2];
	r[1] = a[2] * b[0] - b[2] * a[0];
	r[2] = a[0] * b[1] - b[0] * a[1];
}

void curveRotation(float *der, float *up){

	float left[3];
	
	normalize(der, 3);
	crossProduct(left, up, der);
	normalize(left, 3);
	crossProduct(up, der, left);
	normalize(up, 3);

	float m[16] = { left[0], left[1], left[2], 0, up[0], up[1], up[2], 0, der[0], der[1], der[2], 0,0.0f,0.0f,0.0f, 1 };
	glMultMatrixf(m);
}

void Translation::apply(){
	//Normal Translation
	if(time == 0)
		glTranslatef(x, y, z);
	else{ //Curved and Timed Translation
		float res[3];
		float der[3];

		if(line) renderCatmullRomCurve(res, der, points.size(), points);

		getGlobalCatmullRomPoint(t, res, der, points);

		glTranslatef(res[0], res[1], res[2]);

		curveRotation(der, up);

		t = fmod(glutGet(GLUT_ELAPSED_TIME) / (time*1000.0f), 1);
	}
	
	

}

