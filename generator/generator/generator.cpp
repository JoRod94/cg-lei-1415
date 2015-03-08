// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <point.h>
#include <stdlib.h>

//using namespace std;

void create_plane(float length, float width){
	std::vector<point> points;
	int triangle_point_index[6], i = 0;

	//first triangle
	point p1(0, 0, 0);
	points.push_back(p1);
	triangle_point_index[0];
	point p2(0, 0, length);
	points.push_back(p2);
	triangle_point_index[1];
	point p3(width, 0, length);
	points.push_back(p3);
	triangle_point_index[2];

	//second triangle
	triangle_point_index[2];
	point p4(width, 0, 0);
	points.push_back(p4);
	triangle_point_index[3];
	triangle_point_index[0];

	std::cout << "myvector stores " << int(vec.size()) << " numbers.\n";
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "Too few arguments";
		return 0;
	}
	else if (strcmp(argv[1], "plane") == 0){
		if (argc != 5){
			cout << "Must receive 5 arguments";
			return 0;
		}
		cout << "Generating Plane... \n";
		create_plane(atoi(argv[2]), atoi(argv[3]));
		cout << "Done \n";
	}/*
	else if (strcmp(argv[1], "sphere") == 0){
		if (argc < 5){
			cout << "Too few arguments";
			return 0;
		}
		cout << "Generating Sphere... \n";
		writeSphere(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		cout << "Done \n";
	}
	else if (strcmp(argv[1], "box") == 0){
		if (argc < 5){
			cout << "Too few arguments";
			return 0;
		}
		cout << "Generating Box... \n";
		writeBox(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
		cout << "Done \n";
	}
	else if (strcmp(argv[1], "cone") == 0){
		if (argc < 6){
			cout << "Too few arguments";
			return 0;
		}
		cout << "Generating Cone... \n";
		writeCone(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
		cout << "Done \n";
	}*/
	else
		cout << "Command not recognized";
}

