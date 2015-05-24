#include "stdafx.h"
#include "point.h"
#include "patch.h"
#include "vec3.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include "vertex.h"

using namespace std;


unsigned int lastInd = 0;
vector<vertex> verts;				//model template
vector<point> points;				//deprecated
vector<point> normals;				//deprecated

map<vertex, unsigned int> vMap;		//vertex-index dictionary
map<point,unsigned int> pMap;		//deprecated

vector<point> pOrder;				//model points
vector<unsigned int> indices;		//model point indices
vector<vec3> nOrder;				//model normals -> now a vector


void create_file(const char* filename){
	unsigned int pSize = 3 * pOrder.size();
	unsigned int iSize = indices.size();
	float *pCoords = (float*)malloc(pSize * sizeof(float));
	float *nCoords = (float*)malloc(pSize * sizeof(float));


	for (int j = 0, i = 0, k = 0; j<pOrder.size(); j++) {
		pCoords[i] = pOrder[j].x;
		nCoords[i++] = nOrder[j].x;
		pCoords[i] = pOrder[j].y;
		nCoords[i++] = nOrder[j].y;
		pCoords[i] = pOrder[j].z;
		nCoords[i++] = nOrder[j].z;
	}

	//for (int i = 0; i < iSize; i++)
	//	cout << indices[i] << endl;

	ofstream newFile(string(filename) + ".3d", ios::binary);
	newFile.write((char *)&pSize, sizeof(unsigned int));
	newFile.write((char *)&pCoords[0], pSize*sizeof(float));
	newFile.write((char *)&iSize, sizeof(unsigned int));
	newFile.write((char *)&indices[0], iSize*sizeof(unsigned int));
	newFile.write((char *)&nCoords[0], pSize*sizeof(float));
}

point rotate_point(point p, float angle){
	p.x = p.x * sin(angle);
	p.z = p.z * cos(angle);
	return p;
}

//void put_point(int vInd, float x, float y, float z, float angle, bool rotate){
//	map<point, unsigned int>::iterator it;
//	point p = point(x, y, z);
//	point n = point(normals[vInd].x, normals[vInd].y, normals[vInd].x);
//
//	if (rotate){
//		p = rotate_point(p, angle);
//		n = rotate_point(n, angle);
//	}
//
//
//	if ((it = pMap.find(p)) != pMap.end())
//		indices.push_back(it->second);
//	else{
//		pOrder.push_back(p);
//		nOrder.push_back(n);
//		pMap[p] = lastInd;
//		indices.push_back(lastInd++);
//		cout << ">>> " << n.x << " " << n.y << " " << n.z << " " << endl;
//	}
//
//}

void put_vertex_rot(int vInd, float angle){
	map<vertex, unsigned int>::iterator it;
	point p = point(verts[vInd].p.x, verts[vInd].p.y, verts[vInd].p.z);
	vec3 n = vec3(verts[vInd].n.x, verts[vInd].n.y, verts[vInd].n.z);

	vertex v = vertex(p,n);

	v.rotate(angle);

	if ((it = vMap.find(v)) != vMap.end())
		indices.push_back(it->second);
	else{
		pOrder.push_back(v.p);
		nOrder.push_back(v.n);
		vMap[v] = lastInd;
		indices.push_back(lastInd++);
	}

}

void _put_point(point p){
	map<point, unsigned int>::iterator it;

	if ((it = pMap.find(p)) != pMap.end())
		indices.push_back(it->second);
	else{
		pOrder.push_back(p);
		pMap[p] = lastInd;
		indices.push_back(lastInd++);
	}
}

// calculating the linear interpolation of p0, p1, p2 and p3
// on instance t
point cubic_bezier(float t, point* p) {
	point pf;

	pf.x = pow(1 - t, 3) * p[0].x +
		pow(1 - t, 2) * 3 * t * p[1].x +
		(1 - t) * 3 * pow(t, 2) * p[2].x +
		pow(t, 3) * p[3].x;

	pf.y = pow(1 - t, 3) * p[0].y +
		pow(1 - t, 2) * 3 * t * p[1].y +
		(1 - t) * 3 * pow(t, 2) * p[2].y +
		pow(t, 3) * p[3].y;

	pf.z = pow(1 - t, 3) * p[0].z +
		pow(1 - t, 2) * 3 * t * p[1].z +
		(1 - t) * 3 * pow(t, 2) * p[2].z +
		pow(t, 3) * p[3].z;

	return pf;
}

point cubic_bezier_zz(float t, point* p) {
	point pf;

	pf.x = pow(1 - t, 3) * p[0].x +
		pow(1 - t, 2) * 3 * t * p[1].x +
		(1 - t) * 3 * pow(t, 2) * p[2].x +
		pow(t, 3) * p[3].x;

	pf.y = pow(1 - t, 3) * p[0].z +
		pow(1 - t, 2) * 3 * t * p[1].z +
		(1 - t) * 3 * pow(t, 2) * p[2].z +
		pow(t, 3) * p[3].z;

	pf.z = pow(1 - t, 3) * p[0].y +
		pow(1 - t, 2) * 3 * t * p[1].y +
		(1 - t) * 3 * pow(t, 2) * p[2].y +
		pow(t, 3) * p[3].y;

	return pf;
}

point interpolate_yy(float u, float v, Patch p) {
	int j = 0, w = 0;
	point u_points[4];
	point v_points[4];

	for (int i = 0; i < 16; i++) {
		u_points[j] = p.getPoint(i);

		j = (j + 1) % 4;

		if (j == 0)
			v_points[w++] = cubic_bezier(u, u_points);
	}

	return cubic_bezier(v, v_points);
}

point interpolate_zz(float u, float v, Patch p) {
	int j = 0, w = 0;
	point u_points[4];
	point v_points[4];

	for (int i = 0; i < 16; i++) {
		v_points[j] = p.getPoint(i);

		j = (j + 1) % 4;

		if (j == 0)
			u_points[w++] = cubic_bezier_zz(v, v_points);
	}

	return cubic_bezier(u, u_points);
}

void interpolate(int i, int j, float inc, Patch p, bool z_axis) {
	float u = i * inc,
		v = j * inc,
		next_u = (i + 1) * inc,
		next_v = (j + 1) * inc;

	point p0, p1, p2, p3;

	if (z_axis) {
		p0 = interpolate_zz(u, v, p);
		p1 = interpolate_zz(next_u, v, p);
		p2 = interpolate_zz(next_u, next_v, p);
		p3 = interpolate_zz(u, next_v, p);
	}
	else {
		p0 = interpolate_yy(u, v, p);
		p1 = interpolate_yy(next_u, v, p);
		p2 = interpolate_yy(next_u, next_v, p);
		p3 = interpolate_yy(u, next_v, p);
	}

	_put_point(p0);
	_put_point(p1);
	_put_point(p2);

	_put_point(p2);
	_put_point(p3);
	_put_point(p0);
}


void draw_patch(int tesselation, Patch p, bool axis) {
	float inc = 1.0f / (float)tesselation;
	float u, v, next_u, next_v;

	for (int i = 0; i < tesselation; i++) {
		for (int j = 0; j < tesselation; j++)
			interpolate(i, j, inc, p, axis);
	}
}

vector<string> tokenize(string str, char delim, vector<string> &token) {
	size_t start = str.find_first_not_of(delim);
	size_t end = start;

	while (start != string::npos) {
		end = str.find(delim, start);
		token.push_back(str.substr(start, end - start));
		start = str.find_first_not_of(delim, end);
	}

	return token;

}

Patch read_patch(string filename) {
	ifstream file(filename);

	string line;

	getline(file, line);
	int nr_patches = stoi(line);
	Patch p;
	vector<string> tokens;

	for (int i = 0; i < nr_patches; i++) {
		getline(file, line);
		vector<int> indices;
		tokens.clear();
		tokenize(line, ',', tokens);

		for (int j = 0; j < tokens.size(); j++) {
			int x = stoi(tokens[j]);
			indices.push_back(x);
		}

		p.indices.push_back(indices);
	}

	getline(file, line);
	int nr_points = stoi(line);
	vector<point> points;

	for (int i = 0; i < nr_points; i++) {
		getline(file, line);
		tokens.clear();
		tokenize(line, ',', tokens);
		points.push_back(point(
			stof(tokens[0]),
			stof(tokens[1]),
			stof(tokens[2])
			));
	}
	p.points = points;

	return p;
}

void bezier_surface(int tesselation, string in, string out, bool inverted_axis) {
	Patch p = read_patch(in);

	int size = p.indices.size();

	for (int i = 0; i < size; i++) {
		p.next_patch();
		draw_patch(tesselation, p, inverted_axis);
	}

	create_file( out.c_str() );
}



//void getOuterPoints(float len, float wid, float hei, int stacks, int slices){
//	float incrL = len / slices;
//	float incrH = hei / stacks;
//	float incrW = wid / slices;
//	float startingL = -len / 2, l = startingL;
//	float startingH = -hei / 2, h = startingH;
//	float startingW = -wid / 2, w = startingW;
//	int i;
//
//	for (i = 0; i < slices; i++){
//		points.push_back(point(l, h, 0));
//		l += incrL;
//	}
//	for (i = 0; i < stacks; i++){
//		points.push_back(point(l, h, 0));
//		h += incrH;
//	}
//	for (i = 0; i < slices; i++){
//		points.push_back(point(l, h, 0));
//		l -= incrL;
//	}
//	for (i = 0; i < stacks; i++){
//		points.push_back(point(l, h, 0));
//		h -= incrH;
//	}
//
//}
//
//
//void completeFace(float len, float hei, float position, int slices, int stacks){
//	float incrL = len / slices;
//	float incrH = hei / stacks;
//	float startingL = -len / 2, l = startingL;
//	float startingH = -hei / 2, h = startingH;
//
//	for (int j = 0; j < stacks; j++){
//		for (int i = 0; i < slices; i++){
//			if (position < 0){
//				put_point(i, l, h, position);
//				put_point(i, l, h + incrH, position);
//				put_point(i, l + incrL, h + incrH, position);
//				put_point(i, l, h, position);
//				put_point(i, l + incrL, h + incrH, position);
//				put_point(i, l + incrL, h, position);
//			}
//			else{
//				put_point(i, l, h, position);
//				put_point(i, l + incrL, h + incrH, position);
//				put_point(i, l, h + incrH, position);
//
//				put_point(i, l, h, position);
//				put_point(i, l + incrL, h, position);
//				put_point(i, l + incrL, h + incrH, position);
//			}
//			l += incrL;
//		}
//		h += incrH;
//		l = startingL;
//	}
//}
//
//
//
//
//void pointExtender(float len, float wid, float hei, int stacks, int slices){
//	int numpts = (stacks + 1)*(slices + 1) - (stacks - 1) * (slices - 1);
//	float incrW = wid / slices;
//	float startingW = -wid / 2;
//	float w = startingW;
//	int j;
//
//	completeFace(len, hei, w, slices, stacks);
//	for (int i = 0; i < slices; i++){
//		for (j = 0; j < numpts - 1; j++){
//			put_point(i, points[j].x, points[j].y, w);
//			put_point(i, points[j + 1].x, points[j + 1].y, w);
//			put_point(i, points[j + 1].x, points[j + 1].y, w + incrW);
//
//			put_point(i, points[j].x, points[j].y, w);
//			put_point(i, points[j + 1].x, points[j + 1].y, w + incrW);
//			put_point(i, points[j].x, points[j].y, w + incrW);
//
//		}
//		put_point(i, points[j].x, points[j].y, w);
//		put_point(i, points[0].x, points[0].y, w);
//		put_point(i, points[0].x, points[0].y, w + incrW);
//
//		put_point(i, points[j].x, points[j].y, w);
//		put_point(i, points[0].x, points[0].y, w + incrW);
//		put_point(i, points[j].x, points[j].y, w + incrW);
//
//		w += incrW;
//	}
//	completeFace(len, hei, w, slices, stacks);
//}
//
//
//void create_plane(float length, float width, int slices, int stacks){
//	getOuterPoints(length, 0, width, stacks, slices);
//	completeFace(length, width, 0, slices, stacks);
//}
//
//void create_parallelepiped(float length, float width, float height, int slices, int stacks){
//	getOuterPoints(length, width, height, stacks, slices);
//	pointExtender(length, width, height, stacks, slices);
//}

//Rotates model template to create complete model
void vertex_rotator(float slices, float layers){
	float alpha = (float)M_PI/2.0f;
	float aInc = (2.0f * (float)M_PI) / slices;

	for (int i = 0; i < slices; i++){
		for (int j = 0; j < layers; j++){
			put_vertex_rot(j, alpha);
			put_vertex_rot(j + 1, alpha);
			put_vertex_rot(j, (alpha + aInc));

			put_vertex_rot(j, (alpha + aInc)); 
			put_vertex_rot(j + 1, alpha);
			put_vertex_rot(j + 1, (alpha + aInc));
		}
		alpha += aInc;
	}

}

void create_sphere(float radius, float slices, float layers){
	float beta = 0.0f;
	float bInc = M_PI / layers;

	for (int i = 0; i < layers; i++, beta += bInc)
		verts.push_back(vertex(	point(radius * sin(beta), radius * cos(beta), 0),
								vec3(sin(beta), cos(beta), 0)));

	verts.push_back(vertex(	point(0, -radius, 0),
							vec3(0, -1, 0)));

	vertex_rotator(slices, layers);
}

void create_cone(float radius, float height, float slices, float layers){
	float currR = 0.0f, currH = height;
	float rInc = radius / layers, hDec = height / layers;
	float normalAngle = (atan(height/radius) * (180/M_PI));

	for (int i = 0; i < (layers + 1); i++, currR += rInc, currH -= hDec)
		verts.push_back(vertex(	point(currR, currH, 0),
								vec3(cos(normalAngle), sin(normalAngle), 0)));


	verts.push_back(vertex(	point(radius, 0, 0),
							vec3(0, -1, 0)));
	verts.push_back(vertex(	point(0, 0, 0),
							vec3(0, -1, 0)));

	vertex_rotator(slices, layers + 2);
}

void create_torus(float or, float ir, float slices, float layers){
	float beta = 0.0f;
	float bInc = (2 * (float)M_PI) / layers;

	for (int i = 0; i < (layers + 1); i++, beta += bInc)
	{
		point p = point(or + (ir * sin(beta)), ir * cos(beta), 0);
		verts.push_back(vertex(p,
			vec3(sin(beta), cos(beta), 0)));
		cout << p.x << ", " << p.y << ", " << p.z << endl;
	}

	vertex_rotator(slices, layers);
}

void create_ring(float or, float ir, float slices){
	verts.push_back(vertex(point(ir, 0, 0),vec3(0, 1, 0)));
	verts.push_back(vertex(point(or, 0, 0), vec3(0, 1, 0)));

	vertex_rotator(slices, 1);

	verts.clear();

	verts.push_back(vertex(point(or, 0.0001, 0), vec3(0, -1, 0)));
	verts.push_back(vertex(point(ir, 0.0001, 0), vec3(0, -1, 0)));

	vertex_rotator(slices, 1);
}

void create_cylinder(float radius, float height, float slices, float layers){
	float currH = height / 2;
	float hDec = height / layers;

	verts.push_back(vertex(point(0, currH, 0), vec3(0, 1, 0)));
	verts.push_back(vertex(point(radius, currH, 0), vec3(0, 1, 0)));

	for (int i = 0; i < (layers + 1); i++, currH -= hDec)
		verts.push_back(vertex(point(radius, currH, 0), vec3(1, 0, 0)));

	verts.push_back(vertex(point(radius, currH+hDec, 0), vec3(0, -1, 0)));
	verts.push_back(vertex(point(0, currH+hDec, 0), vec3(0, -1, 0)));

	vertex_rotator(slices, layers + 4);
}

void rand_displace(int aSize, int lastSize, float rD, float rA){
	for (int i = lastSize; i < aSize; i++){
		pOrder[i].x = rD * sin(rA);
		pOrder[i].z = rD * cos(rA);

		cout << pOrder[i].x << endl;
		cout << pOrder[i].z << endl;
	}
}

void create_asteroids(float ir, float or, float nrAsteroids, float radius, float slices, float layers){
	float randDistance, randAngle;
	int lastSize = 0;

	srand(1);

	for (int i = 0; i < nrAsteroids; i++){
		create_sphere(radius, slices, layers);

		randDistance = ir + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (or - ir)));
		randAngle = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*M_PI)));
		cout << randDistance << endl;
		cout << randAngle << endl;
		//rand_disform();

		rand_displace((pOrder.size() - lastSize), lastSize,randDistance,randAngle);
		cout << "asdasdsad"<<pOrder.size() << endl;

		pMap.clear();
	}
}


bool valid_bezier(int argc, char* argv[]){
	int file = 1;
	regex number("[0-9]*\.?[0-9]+");
	regex word("[\\\a-zA-Z0-9\-_./]*[a-zA-Z0-9]");
	
	file = regex_match(argv[argc - 1], word);
	
	return file && regex_match(argv[2], number) && regex_match(argv[3], word);

}

bool valid_input(int argc, char* argv[]) {
	int nrs = 1;
	int file = 1;
	regex number("[0-9]*\.?[0-9]+");
	regex word("[\\\a-zA-Z0-9\-_./]*[a-zA-Z0-9]");

	for (int i = 2; i < argc - 1 && nrs; i++)
		nrs = regex_match(argv[i], number);

	file = regex_match(argv[argc - 1], word);

	return (file && nrs) || valid_bezier(argc, argv);
}

int main(int argc, char* argv[]) {
	if (argc < 2){
		std::cout << "Too few arguments." << endl;
		return 0;
	}
	else if (!valid_input(argc, argv)) {
		std::cout << "Invalid input" << endl;
		return 0;
	}
	else if (strcmp(argv[1], "plane") == 0){
		if (argc != 7){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Plane..." << endl;
		//create_plane(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
	}
	else if (strcmp(argv[1], "bezier") == 0){
		if (argc != 5){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Plane..." << endl;
		bezier_surface(stof(argv[2]), argv[3],argv[4], false);
	}
	else if (strcmp(argv[1], "ibezier") == 0){
		if (argc != 5){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Plane..." << endl;
		bezier_surface(stof(argv[2]), argv[3], argv[4], true);
	}
	else if (strcmp(argv[1], "sphere") == 0){
		if (argc != 6){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Sphere..." << endl;
		create_sphere(stof(argv[2]), stof(argv[3]), stof(argv[4]));
	}
	else if (strcmp(argv[1], "box") == 0){
		if (argc != 8){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Box..." << endl;
		//create_parallelepiped(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), stof(argv[6]));
	}
	else if (strcmp(argv[1], "cone") == 0){
		if (argc != 7){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Cone..." << endl;
		create_cone(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
	}
	else if (strcmp(argv[1], "cylinder") == 0){
		if (argc != 7){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Cylinder..." << endl;
		create_cylinder(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
	}
	else if (strcmp(argv[1], "torus") == 0){
		if (argc != 7){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		std::cout << "Generating Torus..." << endl;
		create_torus(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
	}
	else if (strcmp(argv[1], "ring") == 0){
		if (argc != 6){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		if (stof(argv[2]) < stof(argv[3]) ){
			std::cout << "Inner radius must be smaller than outer radius" << endl;
			return 0;
		}
		std::cout << "Generating Ring..." << endl;
		create_ring(stof(argv[2]), stof(argv[3]), stof(argv[4]));
	}
	else if (strcmp(argv[1], "asteroids") == 0){
		if (argc != 9){
			std::cout << "Wrong number of arguments" << endl;
			return 0;
		}
		if (stof(argv[2]) > stof(argv[3])){
			std::cout << "Inner radius must be smaller than outer radius" << endl;
			return 0;
		}
		std::cout << "Generating Asteroids..." << endl;
		create_asteroids(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), stof(argv[6]), stof(argv[7]));
	}
	else{
		std::cout << "Command not recognized" << endl;
		return 0;
	}

	std::cout << "Writing to file..." << endl;
	create_file(argv[argc - 1]);
	std::cout << "Done" << endl;

	return 0;
}


