// generator.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "point.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

vector<point> points;

void create_file(const char* filename){
	unsigned long int size = points.size();

	ofstream newFile(string(filename) + ".3d", ios::binary);
	newFile.write((char *)&size, sizeof(size));
	newFile.write((char *)&points[0], size*sizeof(point));
}



/*
* Function receives float 'size' (width, length or height), and a float 'div' (number of slices or stacks)
* and returns a vector with the coordinates of all the points according to the given parameters.
*/
vector<float> pointArrays(float size, float div){
	size_t tot = 2 + (div - 1);
	float incr = -(size / 2);
	vector<float> pts(tot);
	for (int i = 0; i < tot; i++){
		pts[i] = incr;
		incr += (size / div);
	}
	return pts;
}

/*
* Function planePoints puts together the values of two vectors to create the points.
* Since the plane is a 2D object, the parameter 'c' completes the points (which are 3D).
*/

// xOy plane facing forward

void YZFwdPlanePoints(float c, float y1, float y2, float z1, float z2){
				point p1 (c, y1, z1);
				point p2 (c, y2, z2);
				point p3 (c, y1, z2);
				point p4 (c, y1, z1);
				point p5 (c, y2, z1);
				point p6(c, y2, z2);

				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			
		
	}

// yOz facing forward
	void YZBwdPlanePoints(float c, float y1, float y2, float z1, float z2){
				point p1(c, y1, z1);
				point p2(c, y1, z2);
				point p3(c, y2, z2);

				point p4(c, y1, z1);
				point p5(c, y2, z2);
				point p6(c, y2, z1);

				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			
}


void XZBwdPlanePoints(float c, float x1, float x2, float z1, float z2){
				point p1(x1, c, z1);
				point p2(x2, c, z2);
				point p3(x1, c, z2);

				point p4(x1, c, z1);
				point p5(x2, c, z1);
				point p6(x2, c, z2);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
}

// xOz plane facing forward
void XZFwdPlanePoints(float c, float x1, float x2, float z1, float z2){
				point p1(x1, c, z1);
				point p2(x1, c, z2);
				point p3(x2, c, z2);
				point p4(x1, c, z1);
				point p5(x2, c, z2);
				point p6(x2, c, z1);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
}

void XYFwdPlanePoints(float c, float x1, float x2, float y1, float y2){
				point p1(x1, y1, c);
				point p2(x2, y2, c);
				point p3(x1, y2, c);
				point p4(x1, y1, c);
				point p5(x2, y1, c);
				point p6(x2, y2, c);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
}

void XYBwdPlanePoints(float c, float x1, float x2, float y1, float y2){
				point p1(x1, y1, c);
				point p2(x1, y2, c);
				point p3(x2, y2, c);

				point p4(x1, y1, c);
				point p5(x2, y2, c);
				point p6(x2, y1, c);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
}
	


/*
* Creates a plane given its length, width, number of columns and number of rows.
* The plane will be xOy, with z = 0.
*/

void create_plane(float length, float width, float columns, float rows){
	vector<float> r = pointArrays(length, rows);
	vector<float> c = pointArrays(width, columns);
	for (int i = 0; i < r.size() - 1; i++)
		for (int j = 0; j < c.size() - 1; j++)
			XYFwdPlanePoints(0.0f, r[i], r[i+1], c[j], c[j+1]);
}


/*
 * Creates a parallelepiped given it's length, width, height, number of slices and number of stacks.
 * Function calls the plane
 */
void create_parallelepiped(float length, float width, float height, float slices, float stacks){
	vector<point> points;
	vector<float> lenpts = pointArrays(length, slices);
	vector<float> widpts = pointArrays(width, slices);
	vector<float> higpts = pointArrays(height, stacks);
	
	// one plane for each side of the parallelepiped
	for (int i = 0; i < lenpts.size() - 1; i++)
		for (int j = 0; j < higpts.size() - 1; j++){
			XYFwdPlanePoints((width / 2), lenpts[i], lenpts[i+1], higpts[j], higpts[j+1]);
			XYBwdPlanePoints(-(width / 2), lenpts[i], lenpts[i + 1], higpts[j], higpts[j + 1]);
		}

	for (int i = 0; i < higpts.size() - 1; i++)
		for (int j = 0; j < widpts.size() -1; j++){
			YZFwdPlanePoints((length / 2), higpts[i], higpts[i+1], widpts[j], widpts[j+1]);
			YZBwdPlanePoints(-(length / 2), higpts[i], higpts[i+1], widpts[j], widpts[j+1]);
		}

	for (int i = 0; i < lenpts.size() - 1; i++)
		for (int j = 0; j < widpts.size() - 1; j++){
			XZFwdPlanePoints((height / 2), lenpts[i], lenpts[i + 1], widpts[j], widpts[j + 1]);
			XZBwdPlanePoints(-(height / 2), lenpts[i], lenpts[i + 1], widpts[j], widpts[j + 1]);
		}


}

void pointRotator(float slices, float layers){
	float alpha = 0.0f, beta = (float)M_PI;
	float aInc = (2.0f * (float)M_PI) / slices;

	for (int i = 0; i < slices; i++){
		for (int j = 0; j < layers; j++){
			points.push_back(point(points[j].x * sin(alpha), points[j].y, points[j].x * cos(alpha)));
			points.push_back(point(points[j].x * sin(alpha + aInc), points[j].y, points[j].x * cos(alpha + aInc)));
			points.push_back(point(points[j + 1].x * sin(alpha + aInc), points[j + 1].y, points[j + 1].x * cos(alpha + aInc)));

			points.push_back(point(points[j + 1].x * sin(alpha + aInc), points[j + 1].y, points[j + 1].x * cos(alpha + aInc)));
			points.push_back(point(points[j + 1].x * sin(alpha), points[j + 1].y, points[j + 1].x * cos(alpha)));
			points.push_back(point(points[j].x * sin(alpha), points[j].y, points[j].x * cos(alpha)));
		}
		alpha += aInc;
	}
}

void create_sphere(float radius, float slices, float layers){
	float beta = (float)M_PI;
	float bInc = (float)M_PI / layers;

	for (int i = 0; i < (layers + 1); i++, beta += bInc)
		points.push_back(point(radius * sin(beta), radius * cos(beta), 0));

	pointRotator(slices, layers);
}

void create_cone(float radius, float height, float slices, float layers){
	float currR = radius, currH = 0.0f;
	float rDec = radius / layers, hInc = height / layers;

	points.push_back(point(0, 0, 0));
	for (int i = 0; i < (layers + 1); i++, currR -= rDec, currH += hInc)
		points.push_back(point(currR, currH, 0));

	pointRotator(slices, layers + 1);
}

void create_torus(float ir, float or, float slices, float layers){
	float beta = 0.0f;
	float bInc = (2 * (float)M_PI) / layers;

	for (int i = 0; i < (layers + 1); i++, beta += bInc)
		points.push_back(point(ir + or * sin(beta), or * cos(beta), 0));

	pointRotator(slices, layers);
}

void create_ring(float ir, float or, float slices){
	points.push_back(point(ir, 0, 0));
	points.push_back(point(ir + (2 * or), 0, 0));
	points.push_back(point(ir, 0, 0));
	points.push_back(point(ir + (2 * or), 0, 0));

	pointRotator(slices, 1);
}

void create_cylinder(float radius, float height, float slices, float layers){
	float currH = -height / 2;
	float hInc = height / layers;

	points.push_back(point(0, currH, 0));
	for (int i = 0; i < (layers + 1); i++, currH += hInc)
		points.push_back(point(radius, currH, 0));
	points.push_back(point(0, currH - hInc, 0));

	pointRotator(slices, layers + 2);
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


