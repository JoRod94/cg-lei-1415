// generator.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "point.h"
#include "patch.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>

using namespace std;

unsigned int lastInd = 0, cp = 0;
map<point,unsigned int> pMap;
vector<point> pOrder;
vector<point> points;
vector<unsigned int> indices;



void create_file(const char* filename){
	unsigned int pSize = 3 * pOrder.size(),
				iSize = indices.size();
	float *pCoords = (float*)malloc(pSize * sizeof(float));



	map<point, unsigned int>::iterator it;
	for (int j = 0, i = 0; j<pOrder.size(); j++) {
		pCoords[i++] = pOrder[j].x;
		pCoords[i++] = pOrder[j].y;
		pCoords[i++] = pOrder[j].z;
	}

	ofstream newFile(string(filename) + ".3d", ios::binary);
	newFile.write((char *)&pSize, sizeof(unsigned int));
	newFile.write((char *)&pCoords[0], pSize*sizeof(float));
	newFile.write((char *)&iSize, sizeof(unsigned int));
	newFile.write((char *)&indices[0], iSize*sizeof(unsigned int));
}


void put_point(float x, float y, float z){
	map<point, unsigned int>::iterator it;
	point p = point(x, y, z);

	if ((it = pMap.find(p)) != pMap.end())
		indices.push_back(it->second);
	else{
		pOrder.push_back(p);
		pMap[p] = lastInd;
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



void getOuterPoints(float len, float wid, float hei, int stacks, int slices){
	float incrL = len / slices;
	float incrH = hei / stacks;
	float incrW = wid / slices;
	float startingL = -len / 2, l = startingL;
	float startingH = -hei / 2, h = startingH;
	float startingW = -wid / 2, w = startingW;
	int i;

	for (i = 0; i < slices; i++){
		points.push_back(point(l, h, 0));
		l += incrL;
	}
	for (i = 0; i < stacks; i++){
		points.push_back(point(l, h, 0));
		h += incrH;
	}
	for (i = 0; i < slices; i++){
		points.push_back(point(l, h, 0));
		l -= incrL;
	}
	for (i = 0; i < stacks; i++){
		points.push_back(point(l, h, 0));
		h -= incrH;
	}

}


void completeFace(float len, float hei, float position, int slices, int stacks){
	float incrL = len / slices;
	float incrH = hei / stacks;
	float startingL = -len / 2, l = startingL;
	float startingH = -hei / 2, h = startingH;

	for (int j = 0; j < stacks; j++){
		for (int i = 0; i < slices; i++){
			if (position < 0){
				put_point(l, h, position);
				put_point(l, h + incrH, position);
				put_point(l + incrL, h + incrH, position);
				put_point(l, h, position);
				put_point(l + incrL, h + incrH, position);
				put_point(l + incrL, h, position);
			}
			else{
				put_point(l, h, position);
				put_point(l + incrL, h + incrH, position);
				put_point(l, h + incrH, position);

				put_point(l, h, position);
				put_point(l + incrL, h, position);
				put_point(l + incrL, h + incrH, position);
			}
			l += incrL;
		}
		h += incrH;
		l = startingL;
	}
}




void pointExtender(float len, float wid, float hei, int stacks, int slices){
	int numpts = (stacks + 1)*(slices + 1) - (stacks - 1) * (slices - 1);
	float incrW = wid / slices;
	float startingW = -wid / 2;
	float w = startingW;
	int j;

	completeFace(len, hei, w, slices, stacks);
	for (int i = 0; i < slices; i++){
		for (j = 0; j < numpts - 1; j++){
			put_point(points[j].x, points[j].y, w);
			put_point(points[j + 1].x, points[j + 1].y, w);
			put_point(points[j + 1].x, points[j + 1].y, w + incrW);
			put_point(points[j].x, points[j].y, w);
			put_point(points[j + 1].x, points[j + 1].y, w + incrW);
			put_point(points[j].x, points[j].y, w + incrW);

		}
		put_point(points[j].x, points[j].y, w);
		put_point(points[0].x, points[0].y, w);
		put_point(points[0].x, points[0].y, w + incrW);

		put_point(points[j].x, points[j].y, w);
		put_point(points[0].x, points[0].y, w + incrW);
		put_point(points[j].x, points[j].y, w + incrW);

		w += incrW;
	}
	completeFace(len, hei, w, slices, stacks);
}


void create_plane(float length, float width, int slices, int stacks){
	getOuterPoints(length, 0, width, stacks, slices);
	completeFace(length, width, 0, slices, stacks);
}

void create_parallelepiped(float length, float width, float height, int slices, int stacks){
	getOuterPoints(length, width, height, stacks, slices);
	pointExtender(length, width, height, stacks, slices);

}

void point_rotator(float slices, float layers){
	float alpha = (float)M_PI/2.0f;
	float aInc = (2.0f * (float)M_PI) / slices;

	for (int i = 0; i < slices; i++){
		for (int j = 0; j < layers; j++){
			put_point(points[j].x * sin(alpha), points[j].y, points[j].x * cos(alpha));
			put_point(points[j].x * sin(alpha + aInc), points[j].y, points[j].x * cos(alpha + aInc));
			put_point(points[j + 1].x * sin(alpha + aInc), points[j + 1].y, points[j + 1].x * cos(alpha + aInc));

			put_point(points[j + 1].x * sin(alpha + aInc), points[j + 1].y, points[j + 1].x * cos(alpha + aInc));
			put_point(points[j + 1].x * sin(alpha), points[j + 1].y, points[j + 1].x * cos(alpha));
			put_point(points[j].x * sin(alpha), points[j].y, points[j].x * cos(alpha));
		}
		alpha += aInc;
	}
}

/*void point_rotator(float slices, float layers){
	float aInc = (2.0f * (float)M_PI) / slices, alpha = aInc;
	int ind = 0;

	for (int i = 0; i < slices; i++){
		for (int j = 0; j < layers; j++){
			ind = (layers+1)*i + j;

			points.push_back(point(points[j].x * sin(alpha), points[j].y, points[j].x * cos(alpha)));

			indices.push_back(ind);
			indices.push_back(ind + layers + 1);
			indices.push_back(ind + layers + 2);

			indices.push_back(ind + layers + 2);
			indices.push_back(ind + 1);
			indices.push_back(ind);
		}
		points.push_back(point(points[layers].x * sin(alpha), points[layers].y, points[layers].x * cos(alpha)));
		alpha += aInc;
	}
}*/

void create_sphere(float radius, float slices, float layers){
	float beta = 0.0f;
	float bInc = M_PI / layers;

	for (int i = 0; i < layers; i++, beta += bInc)
		points.push_back(point(radius * sin(beta), radius * cos(beta), 0));

	points.push_back(point(0,-1, 0));

	point_rotator(slices, layers);
}

void create_cone(float radius, float height, float slices, float layers){
	float currR = radius, currH = 0.0f;
	float rDec = radius / layers, hInc = height / layers;

	points.push_back(point(0, 0, 0));
	for (int i = 0; i < (layers + 1); i++, currR -= rDec, currH += hInc)
		points.push_back(point(currR, currH, 0));

	point_rotator(slices, layers + 1);
}

void create_torus(float ir, float or, float slices, float layers){
	float beta = 0.0f;
	float bInc = (2 * (float)M_PI) / layers;

	for (int i = 0; i < (layers + 1); i++, beta += bInc)
		points.push_back(point(ir + or * sin(beta), or * cos(beta), 0));

	point_rotator(slices, layers);
}

void create_ring(float ir, float or, float slices){
	points.push_back(point(ir, 0, 0));
	points.push_back(point(ir + (2 * or), 0, 0));

	point_rotator(slices, 1);

	points.clear();

	points.push_back(point(ir + (2 * or), 0, 0));
	points.push_back(point(ir, 0, 0));
	point_rotator(slices, 1);
}

void create_cylinder(float radius, float height, float slices, float layers){
	float currH = -height / 2;
	float hInc = height / layers;

	points.push_back(point(0, currH, 0));
	for (int i = 0; i < (layers + 1); i++, currH += hInc)
		points.push_back(point(radius, currH, 0));
	points.push_back(point(0, currH - hInc, 0));

	point_rotator(slices, layers + 2);
}

int valid_input(int argc, char* argv[]) {
	int nrs = 1;
	int file = 1;
	regex number("[0-9]*\.?[0-9]+");
	regex word("[a-zA-Z0-9\-_\.]*[a-zA-Z0-9]");

	for (int i = 2; i < argc - 1 && nrs; i++)
		nrs = regex_match(argv[i], number);

	file = regex_match(argv[argc - 1], word);

	return file && nrs;
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
		create_plane(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]));
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
		create_parallelepiped(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), stof(argv[6]));
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
		if (stof(argv[2]) > stof(argv[3]) ){
			std::cout << "Inner radius must be smaller than outer radius" << endl;
			return 0;
		}
		std::cout << "Generating Ring..." << endl;
		create_ring(stof(argv[2]), stof(argv[3]), stof(argv[4]));
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


