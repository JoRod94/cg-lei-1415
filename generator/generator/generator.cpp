// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <point.h>
#include <stdlib.h>

using namespace std;

void create_file(const char* filename, vector<point> points, int *tpi, int tpi_size){
	unsigned long int size = points.size();

	ofstream newFile("testing3.binary", ios::binary);
	printf("SIZE: %lu\n", size);
	newFile.write((char *)&size, sizeof(size));
	newFile.write((char *)&points[0], size*sizeof(point));

	/*
	fwrite(&points, sizeof(points[0]), sizeof(points.size()), newFile);
	fwrite(tpi, sizeof(int), sizeof(tpi_size), newFile);

	if (fopen_s(&f, filename, "w") == 0){
		f.write(&size, sizeof(long));
		fwrite(&points, sizeof(points[0]), sizeof(points.size()), f);
		fwrite(tpi, sizeof(int), sizeof(tpi_size), f);
		//fprintf(f, "Teste String YAY!!\n chipiyay \tmadafacas.");
		fclose(f);
	}
	else
		cout << "Could not create specified file\n";*/
}


void create_plane(float length, float width, vector<point> &points, int tpi[6]){
	int i = 0;

void create_plane(float length, float width, vector<point> points, int tpi[6]){
	//first triangle
	point p1(0, 0, 0);
	points.push_back(p1);
	tpi[0];
	point p2(0, 0, length);
	points.push_back(p2);
	tpi[1];
	point p3(width, 0, length);
	points.push_back(p3);
	tpi[2];

	//second triangle
	tpi[2];
	point p4(width, 0, 0);
	points.push_back(p4);
	tpi[3];
	tpi[0];
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "Too few arguments\n";
		return 0;
	}
	else if (strcmp(argv[1], "plane") == 0){
		if (argc != 5){
			cout << "Must receive 5 arguments\n";
			return 0;
		}
		cout << "Generating Plane...\n";
		std::vector<point> points;
		int triangle_point_index[6];
		create_plane(atoi(argv[2]), atoi(argv[3]), points, triangle_point_index);
		cout << "Writing to file...\n";
		create_file(argv[4], points, triangle_point_index, 4);
		cout << "Done\n";
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

