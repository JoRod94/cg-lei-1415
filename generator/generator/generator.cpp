// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "point.h"
#include <stdlib.h>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

vector<point> points;

void create_file(const char* filename){
	unsigned long int size = points.size();

	ofstream newFile(string(filename) + ".3d", ios::binary);
	printf("%lu\n", size);
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

void create_plane(float length, float width, int columns, int rows){
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

void create_sphere(float radius, float nSlices, float nLayers){
	vector<int> indexes;

	float sliceInc = (2.0f * ((float)M_PI)) / nSlices;
	float layerInc = (((float)M_PI) / 2.0f) / nLayers;

	//alfa: horizontal angle; beta vertical angle;
	float alfa = 0.0f, beta = 0.0f;

	for (int i = 0; i < nLayers; i++){

		alfa = 0.0f;

		for (int j = 0; j < nSlices; j++){
			//top half triangles
			points.push_back(point(radius * sin(alfa) * cos(beta), radius * sin(beta), radius * cos(alfa) * cos(beta)));
			points.push_back(point(radius * sin(alfa + sliceInc) * cos(beta), radius * sin(beta), radius * cos(alfa + sliceInc) * cos(beta)));
			points.push_back(point(radius * sin(alfa + sliceInc) * cos(beta + layerInc), radius * sin(beta + layerInc), radius * cos(alfa + sliceInc) * cos(beta + layerInc)));

			points.push_back(point(radius * sin(alfa + sliceInc) * cos(beta + layerInc), radius * sin(beta + layerInc), radius * cos(alfa + sliceInc) * cos(beta + layerInc)));
			points.push_back(point(radius * sin(alfa) * cos(beta + layerInc), radius * sin(beta + layerInc), radius * cos(alfa) * cos(beta + layerInc)));
			points.push_back(point(radius * sin(alfa) * cos(beta), radius * sin(beta), radius * cos(alfa) * cos(beta)));

			//bottom half triangles
			points.push_back(point(radius * sin(alfa) * cos(-beta - layerInc), radius * sin(-beta - layerInc), radius * cos(alfa) * cos(-beta - layerInc)));
			points.push_back(point(radius * sin(alfa + sliceInc) * cos(-beta - layerInc), radius * sin(-beta - layerInc), radius * cos(alfa + sliceInc) * cos(-beta - layerInc)));
			points.push_back(point(radius * sin(alfa + sliceInc) * cos(-beta), radius * sin(-beta), radius * cos(alfa + sliceInc) * cos(-beta)));

			points.push_back(point(radius * sin(alfa + sliceInc) * cos(-beta), radius * sin(-beta), radius * cos(alfa + sliceInc) * cos(-beta)));
			points.push_back(point(radius * sin(alfa) * cos(-beta), radius * sin(-beta), radius * cos(alfa) * cos(-beta)));
			points.push_back(point(radius * sin(alfa) * cos(-beta - layerInc), radius * sin(-beta - layerInc), radius * cos(alfa) * cos(-beta - layerInc)));

			alfa += sliceInc;
		}

		beta += layerInc;
	}
}

void create_cylinder(float radius, float height, float slices, float layers){
	float currAng = 0.0f;
	float currH;
	float angInc = (2 * ((float)M_PI)) / slices;
	float hInc = height / layers;

	//cada fatia
	for (int i = 1; i <= slices; i++){
		//triangulo do fundo
		points.push_back(point(0, -(height / 2), 0));
		points.push_back(point(sin(currAng + angInc)*radius, -(height / 2), cos(currAng + angInc)*radius));
		points.push_back(point(sin(currAng)*radius, -(height / 2), cos(currAng)*radius));
		//triangulo do topo
		points.push_back(point(0, (height / 2), 0));
		points.push_back(point(sin(currAng)*radius, (height / 2), cos(currAng)*radius));
		points.push_back(point(sin(currAng + angInc)*radius, (height / 2), cos(currAng + angInc)*radius));
		currH = -(height / 2);
		// rectangulo lateral
		for (int j = 1; j <= layers; j++){

			// rectangulo lateral: triangulo de baixo
			points.push_back(point(sin(currAng)*radius, currH, cos(currAng)*radius));
			points.push_back(point(sin(currAng + angInc)*radius, currH, cos(currAng + angInc)*radius));
			points.push_back(point(sin(currAng + angInc)*radius, currH + hInc, cos(currAng + angInc)*radius));

			// rectangulo lateral: triangulo do topo
			points.push_back(point(sin(currAng + angInc)*radius, currH + hInc, cos(currAng + angInc)*radius));
			points.push_back(point(sin(currAng)*radius, currH + hInc, cos(currAng)*radius));
			points.push_back(point(sin(currAng)*radius, currH, cos(currAng)*radius));

			currH += hInc;
		}
		currAng += angInc;

	}
}

void create_cone(float radius, float height, float nSlices, float nLayers){
	float sliceInc = (2.0f * ( (float) M_PI)) / nSlices;
	float layerInc = ( (float) M_PI / 2.0f) / nLayers;
	float heightInc = height / nLayers;
	float radiusDec = radius / nLayers;
	float alfa = 0.0f, beta = 0.0f, currentRadius = radius, nextRadius = radius, innerRadius = radius, currentheight = 0.0f;

	//cone base
	for (int i = 0; i < nSlices; i++){
		points.push_back(point(radius * sin(alfa), 0.0f, radius * cos(alfa)));
		points.push_back(point(0.0f, 0.0f, 0.0f));
		points.push_back(point(radius * sin(alfa + sliceInc), 0.0f, radius * cos(alfa + sliceInc)));

		alfa += sliceInc;
	}

	//rest of the cone
	currentRadius = innerRadius;
	for (int i = 0; i < nLayers; i++){
		//we need this, otherwise we would get a sphere
		nextRadius = (innerRadius - radiusDec) / cos(beta + layerInc);
		alfa = 0.0f;

		for (int j = 0; j < nSlices; j++){
			//first triangle
			points.push_back(point(currentRadius * sin(alfa) * cos(beta), currentheight, currentRadius * cos(alfa) * cos(beta)));
			points.push_back(point(currentRadius * sin(alfa + sliceInc) * cos(beta), currentheight, currentRadius * cos(alfa + sliceInc) * cos(beta)));
			points.push_back(point(nextRadius * sin(alfa + sliceInc) * cos(beta + layerInc), currentheight + heightInc, nextRadius * cos(alfa + sliceInc) * cos(beta + layerInc)));

			//second triangle
			points.push_back(point(nextRadius * sin(alfa + sliceInc) * cos(beta + layerInc), currentheight + heightInc, nextRadius * cos(alfa + sliceInc) * cos(beta + layerInc)));
			points.push_back(point(nextRadius * sin(alfa) * cos(beta + layerInc), currentheight + heightInc, nextRadius * cos(alfa) * cos(beta + layerInc)));
			points.push_back(point(currentRadius * sin(alfa) * cos(beta), currentheight, currentRadius * cos(alfa) * cos(beta)));

			alfa += sliceInc;
		}

		currentheight += heightInc;
		currentRadius = nextRadius;
		innerRadius -= radiusDec;
		beta += layerInc;
	}
}

void create_torus(float ir, float or, float nRings, float nLayers){
	int i, j;
	point o1, o2, o3;
	float radius = ir + or;
	float layerInc = (2.0f * ((float)M_PI)) / nLayers;
	float ringInc = (2.0f * ((float)M_PI)) / nRings;
	float alfa = 0.0f, beta = 0.0f;

	for (i = 0; i < nRings; i++){
		//calculate 2 reference points for torus ring point generation
		o1 = point(radius * sin(alfa) * cos(beta), 0, radius * cos(alfa) * cos(beta));
		o2 = point(radius * sin(alfa + ringInc) * cos(beta), 0, radius * cos(alfa + ringInc) * cos(beta));

		for (j = 0; j < nLayers; j++){
			points.push_back(point(o1.x + or * sin(alfa) * cos(beta), or * sin(beta), o1.z + or * cos(alfa) * cos(beta)));
			points.push_back(point(o2.x + or * sin(alfa + ringInc) * cos(beta), or * sin(beta), o2.z + or * cos(alfa + ringInc) * cos(beta)));
			points.push_back(point(o2.x + or * sin(alfa + ringInc) * cos(beta + layerInc), or * sin(beta + layerInc), o2.z + or * cos(alfa + ringInc) * cos(beta + layerInc)));

			points.push_back(point(o2.x + or * sin(alfa + ringInc) * cos(beta + layerInc), or * sin(beta + layerInc), o2.z + or * cos(alfa + ringInc) * cos(beta + layerInc)));
			points.push_back(point(o1.x + or * sin(alfa) * cos(beta + layerInc), or * sin(beta + layerInc), o1.z + or * cos(alfa) * cos(beta + layerInc)));
			points.push_back(point(o1.x + or * sin(alfa) * cos(beta), or * sin(beta), o1.z + or * cos(alfa) * cos(beta)));

			beta += layerInc;
		}

		alfa += ringInc;
	}
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
	else{
		std::cout << "Command not recognized" << endl;
		return 0;
	}
	std::cout << "Writing to file..." << endl;
	create_file(argv[argc - 1]);
	std::cout << "Done" << endl;

	return 0;
}


