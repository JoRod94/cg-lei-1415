#include "stdafx.h"
#include "cFrustum.h"
#include <math.h>

#include <GL/glut.h>

#define ANG2RAD 3.14159265358979323846/180.0


cFrustum::cFrustum() {}

cFrustum::~cFrustum() {}

void cFrustum::setCamInternals(float angle, float ratio, float nearD, float farD) {
	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle* ANG2RAD * 0.5);
	nh = nearD * tang;
	nw = nh * ratio;
	fh = farD  * tang;
	fw = fh * ratio;
}


void cFrustum::setCamDef(Vec3 &p, Vec3 &l, Vec3 &u) {

	Vec3 dir, nc, fc, X, Y, Z;

	Z = p - l;
	Z.normalize();

	X = u * Z;
	X.normalize();

	Y = Z * X;

	nc = p - Z * nearD;
	fc = p - Z * farD;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}


int cFrustum::pointInFrustum(Vec3 &p) {
	int result = INSIDE;
	for (int i = 0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);
}


int cFrustum::sphereInFrustum(Vec3 &p, float raio) {
	int result = INSIDE;
	float distance;

	for (int i = 0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result = INTERSECT;
	}
	return(result);

}





void cFrustum::drawPoints() {
	glBegin(GL_POINTS);

	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glVertex3f(nbr.x, nbr.y, nbr.z);

	glVertex3f(ftl.x, ftl.y, ftl.z);
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);

	glEnd();
}


void cFrustum::drawLines() {

	glBegin(GL_LINE_LOOP);
	//near plane
	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//far plane
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//bottom plane
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//top plane
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//left plane
	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	// right plane
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);

	glEnd();
}


void cFrustum::drawPlanes() {

	glBegin(GL_QUADS);

	//near plane
	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(nbl.x, nbl.y, nbl.z);

	//far plane
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);

	//bottom plane
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);

	//top plane
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);
	glVertex3f(ftr.x, ftr.y, ftr.z);

	//left plane

	glVertex3f(ntl.x, ntl.y, ntl.z);
	glVertex3f(nbl.x, nbl.y, nbl.z);
	glVertex3f(fbl.x, fbl.y, fbl.z);
	glVertex3f(ftl.x, ftl.y, ftl.z);

	// right plane
	glVertex3f(nbr.x, nbr.y, nbr.z);
	glVertex3f(ntr.x, ntr.y, ntr.z);
	glVertex3f(ftr.x, ftr.y, ftr.z);
	glVertex3f(fbr.x, fbr.y, fbr.z);

	glEnd();

}

void cFrustum::drawNormals() {

	Vec3 a, b;

	glBegin(GL_LINES);

	// near
	a = (ntr + ntl + nbr + nbl) * 0.25;
	b = a + pl[NEARP].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// far
	a = (ftr + ftl + fbr + fbl) * 0.25;
	b = a + pl[FARP].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// left
	a = (ftl + fbl + nbl + ntl) * 0.25;
	b = a + pl[LEFT].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// right
	a = (ftr + nbr + fbr + ntr) * 0.25;
	b = a + pl[RIGHT].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// top
	a = (ftr + ftl + ntr + ntl) * 0.25;
	b = a + pl[TOP].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	// bottom
	a = (fbr + fbl + nbr + nbl) * 0.25;
	b = a + pl[BOTTOM].normal;
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);

	glEnd();
}


bool cFrustum::gluInvertMatrix(const float m[16], float invOut[16]) {
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}

Vec3 cFrustum::getWorldCoords(float m1[16], float m2[16]) {
	float res[16], aux = 0;

	for (int row = 0; row < 4; row++){

		for (int col = 0; col < 4; col++){

			for (int elem = 0; elem < 4; elem++)
				aux += m1[(4 * row) + elem] * m2[col + (4 * elem)];


			res[(row*4) + col] = aux;
			aux = 0;
		}

		aux = 0;
	}

	return Vec3(res[12], res[13], res[14]);
}