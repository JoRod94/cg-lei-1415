// engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "point.h"

using namespace std;

point *points;

int bin_dec(char * bin, int size)
{
	int nr = 0, pow = 1;
	for (int i = size; i >= 0; --i, pow <<= 1)
		nr += ( (int) bin[i]) * pow;
	return nr;
}

void read_bin(string filename){
	ifstream file;
	char * sizeLine;

	file.open(filename);
	file.read( sizeLine, sizeof(long));

	int arraySize = bin_dec(sizeLine, sizeof(long));

	points = (point *) malloc(arraySize*sizeof(point));

	file.seekg(sizeof(long));

	file.read( (char *) points, arraySize*sizeof(point));
}	


void read_file(string name) {
	ifstream file;
	file.open(name);

	string line,
		   drawing,
		   _nr_points;

	getline(file, line);

	istringstream ss(line);

	getline(ss, drawing, ',');
	getline(ss, _nr_points);

	int nr_points = stoi(_nr_points);

	if (nr_points % 3) {
		std::cout << "Invalid number of points" << endl;
		return;
	}

	for (int i = 0; i < nr_points; i += 3) {
		float x, y, z;
		string token;
		getline(file, line);
		istringstream stream(line);
		getline(stream, token, ',');
		x = stof(token);
		getline(stream, token, ',');
		y = stof(token);
		getline(stream, token);
		z = stof(token);
		point p(x, y, z);
		cout << x << " " << y << " " << z << endl;
	}

	cout << "NR POINTS READ: " << nr_points << " TYPE OF FIGURE: " << drawing << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Hello world! This is engine! Reading from test.txt" << endl;
	read_file("test.txt");
	return 0;
}

