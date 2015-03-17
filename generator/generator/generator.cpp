// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include "point.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

vector<point> points;

void create_file(const char* filename){
	unsigned long int size = points.size();

	ofstream newFile(filename, ios::binary);
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
* Since the plane is a 2D object, the parameters 'o' and 'c' are there to complete the points (which are 3D).
* 'o' has valid values "x", "y", "z". 'c' is the value of that coordinate.
*/
void planePoints(char o, float c, vector<float> xs, vector<float> ys){


	if (o == 'x'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(c, xs[i], ys[j]);
				point p2(c, xs[i + 1], ys[j + 1]);
				point p3(c, xs[i], ys[j + 1]);

				point p4(c, xs[i], ys[j]);
				point p5(c, xs[i + 1], ys[j]);
				point p6(c, xs[i + 1], ys[j + 1]);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			}
		}
	}
	else if (o == 'X'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(c, xs[i], ys[j]);
				point p2(c, xs[i], ys[j + 1]);
				point p3(c, xs[i + 1], ys[j + 1]);

				point p4(c, xs[i], ys[j]);
				point p5(c, xs[i + 1], ys[j + 1]);
				point p6(c, xs[i + 1], ys[j]);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			}
		}
	}
	else if (o == 'Y'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(xs[i], c, ys[j]);
				point p2(xs[i + 1], c, ys[j + 1]);
				point p3(xs[i], c, ys[j + 1]);

				point p4(xs[i], c, ys[j]);
				point p5(xs[i + 1], c, ys[j]);
				point p6(xs[i + 1], c, ys[j + 1]);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			}
		}
	}
	else if (o == 'y'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(xs[i], c, ys[j]);
				point p2(xs[i], c, ys[j + 1]);
				point p3(xs[i + 1], c, ys[j + 1]);

				point p4(xs[i], c, ys[j]);
				point p5(xs[i + 1], c, ys[j + 1]);
				point p6(xs[i + 1], c, ys[j]);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);




			}
		}
	}
	else if (o == 'z'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(xs[i], ys[j], c);
				point p2(xs[i + 1], ys[j + 1], c);
				point p3(xs[i], ys[j + 1], c);

				point p4(xs[i], ys[j], c);
				point p5(xs[i + 1], ys[j], c);
				point p6(xs[i + 1], ys[j + 1], c);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			}
		}
	}
	else if (o == 'Z'){
		for (int i = 0; i < xs.size() - 1; i++){
			for (int j = 0; j < ys.size() - 1; j++){
				point p1(xs[i], ys[j], c);
				point p2(xs[i], ys[j + 1], c);
				point p3(xs[i + 1], ys[j + 1], c);

				point p4(xs[i], ys[j], c);
				point p5(xs[i + 1], ys[j + 1], c);
				point p6(xs[i + 1], ys[j], c);


				points.push_back(p1);
				points.push_back(p2);
				points.push_back(p3);
				points.push_back(p4);
				points.push_back(p5);
				points.push_back(p6);
			}
		}
	}

}

/*
* Creates a plane given its length, width, number of columns and number of rows.
* The plane will be xOy, with z = 0.
*/

void create_plane(float length, float width, int columns, int rows){
	vector<float> r = pointArrays(length, rows);
	vector<float> c = pointArrays(width, columns);

	planePoints('z', 0.0f, r, c);
}


/*
* Creates a parallelepiped given it's length, width, height, number of slices and number of stacks.
* Function calls the plane
*/
void create_parallelepipep(float length, float width, float height, int slices, int stacks){
	vector<float> lenpts = pointArrays(length, slices);
	vector<float> widpts = pointArrays(width, slices);
	vector<float> higpts = pointArrays(height, stacks);

	// one plane for each side of the parallelepiped
	planePoints('z', (width / 2), lenpts, higpts);
	planePoints('x', (length / 2), higpts, widpts);
	planePoints('y', (height / 2), lenpts, widpts);
	planePoints('Z', -(width / 2), lenpts, higpts);
	planePoints('X', -(length / 2), higpts, widpts);
	planePoints('Y', -(height / 2), lenpts, widpts);

}


void draw_sphere(float ray, float nSlices, float nLayers ){
	vector<int> indexes;

	float sliceInc = (2.0f * M_PI) / nSlices;
	float layerInc = (M_PI / 2.0f) / nLayers;

	//alfa: horizontal angle; beta vertical angle;
	float alfa = 0.0f, beta = 0.0f;

	for (int i = 0; i < nLayers; i++){

		alfa = 0.0f;
		for (int j = 0; j < nSlices; j++){
		//	glColor3f(1, 0, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(ray * sin(alfa) * cos(beta), ray * sin(beta), ray * cos(alfa) * cos(beta)));
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(beta), ray * sin(beta), ray * cos(alfa + sliceInc) * cos(beta)));
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(beta + layerInc), ray * sin(beta + layerInc), ray * cos(alfa + sliceInc) * cos(beta + layerInc)));
		//	glEnd();

		//	glColor3f(0, 1, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(beta + layerInc), ray * sin(beta + layerInc), ray * cos(alfa + sliceInc) * cos(beta + layerInc)));
			points.push_back(point(ray * sin(alfa) * cos(beta + layerInc), ray * sin(beta + layerInc), ray * cos(alfa) * cos(beta + layerInc)));
			points.push_back(point(ray * sin(alfa) * cos(beta), ray * sin(beta), ray * cos(alfa) * cos(beta)));
		//	glEnd();

		//	glColor3f(1, 0, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(ray * sin(alfa) * cos(-beta), ray * sin(-beta), ray * cos(alfa) * cos(-beta)));
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(-beta), ray * sin(-beta), ray * cos(alfa + sliceInc) * cos(-beta)));
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(-beta - layerInc), ray * sin(-beta - layerInc), ray * cos(alfa + sliceInc) * cos(-beta - layerInc)));
		//	glEnd();

		//	glColor3f(0, 1, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(ray * sin(alfa + sliceInc) * cos(-beta - layerInc), ray * sin(-beta - layerInc), ray * cos(alfa + sliceInc) * cos(-beta - layerInc)));
			points.push_back(point(ray * sin(alfa) * cos(-beta - layerInc), ray * sin(-beta - layerInc), ray * cos(alfa) * cos(-beta - layerInc)));
			points.push_back(point(ray * sin(alfa) * cos(-beta), ray * sin(-beta), ray * cos(alfa) * cos(-beta)));
		//	glEnd();

			alfa += sliceInc;
		}
		beta += layerInc;
	}
}

void draw_cone(float ray, float height, float nSlices, float nLayers){
	float sliceInc = (2.0f * M_PI) / nSlices;
	float layerInc = (M_PI / 2.0f) / nLayers;
	float heightInc = height / nLayers;
	float rayDec = ray / nLayers;
	float alfa = 0.0f, beta = 0.0f, currentRay = ray, nextRay = ray, innerRay = ray, currentheight = 0.0f;

	//cone base
	for (int i = 0; i < nSlices; i++){
		// glBegin(GL_TRIANGLES);
		points.push_back(point(ray * sin(alfa), 0.0f, ray * cos(alfa)));
		points.push_back(point(0.0f, 0.0f, 0.0f));
		points.push_back(point(ray * sin(alfa + sliceInc), 0.0f, ray * cos(alfa + sliceInc)));
		//glEnd();

		alfa += sliceInc;
	}

	//rest of the cone

	currentRay = innerRay;
	for (int i = 0; i < nLayers; i++){
		//we need this, otherwise we would get a sphere
		nextRay = (innerRay - rayDec) / cos(beta + layerInc);
		alfa = 0.0f;

		for (int j = 0; j < nSlices; j++){
			//first triangle
		//	glColor3f(1, 0, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(currentRay * sin(alfa) * cos(beta), currentheight, currentRay * cos(alfa) * cos(beta)));
			points.push_back(point(currentRay * sin(alfa + sliceInc) * cos(beta), currentheight, currentRay * cos(alfa + sliceInc) * cos(beta)));
			points.push_back(point(nextRay * sin(alfa + sliceInc) * cos(beta + layerInc), currentheight + heightInc, nextRay * cos(alfa + sliceInc) * cos(beta + layerInc)));
		//	glEnd();

			//second triangle
		//	glColor3f(0, 1, 0);
		//	glBegin(GL_TRIANGLES);
			points.push_back(point(nextRay * sin(alfa + sliceInc) * cos(beta + layerInc), currentheight + heightInc, nextRay * cos(alfa + sliceInc) * cos(beta + layerInc)));
			points.push_back(point(nextRay * sin(alfa) * cos(beta + layerInc), currentheight + heightInc, nextRay * cos(alfa) * cos(beta + layerInc)));
			points.push_back(point(currentRay * sin(alfa) * cos(beta), currentheight, currentRay * cos(alfa) * cos(beta)));
		//	glEnd();

			alfa += sliceInc;
		}

		currentheight += heightInc;
		currentRay = nextRay;
		innerRay -= rayDec;
		beta += layerInc;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "Too few arguments\n";
		return 0;
	}
	else if (strcmp(argv[1], "plane") == 0){
		if (argc != 7){
			cout << "Wrong number of arguments\n";
			return 0;
		}
		cout << "Generating Plane...\n";
		create_plane(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
	}
	else if (strcmp(argv[1], "sphere") == 0){
		if (argc != 6){
			cout << "Wrong number of arguments";
			return 0;
		}
		cout << "Generating Sphere... \n";
		draw_sphere(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
	}
	else if (strcmp(argv[1], "box") == 0){
		if (argc != 8){
			cout << "Wrong number of arguments";
			return 0;
		}
		cout << "Generating Box... \n";
		create_parallelepipep(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
	}
	else if (strcmp(argv[1], "cone") == 0){
		if (argc != 7){
			cout << "Wrong number of arguments";
			return 0;
		}
		cout << "Generating Cone... \n";
		draw_cone(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
	}
	else{
		cout << "Command not recognized";
		return 0;
	}
	cout << "Writing to file...\n";
	create_file(argv[argc-1]);
	cout << "Done\n";

	return 0;
}

