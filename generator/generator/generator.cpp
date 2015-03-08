// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <point.h>
#include <stdlib.h>

//using namespace std;

void create_plane(float length, float width, std::vector<float>& vec){
	//first triangle
	point p1(0, 0, 0);
	p1.add_to(vec);
	point p2(0, 0, length);
	p2.add_to(vec);
	point p3(width, 0, length);
	p3.add_to(vec);

	//second triangle
	point p4(width, 0, length);
	p4.add_to(vec);
	point p5(width, 0, 0);
	p5.add_to(vec);
	point p6(0, 0, 0);
	p6.add_to(vec);
	std::cout << "myvector stores " << int(vec.size()) << " numbers.\n";
}

int main(int argc, char* argv[])
{
	if (argc < 2){
		cout << "Too few arguments";
		return 0;
	}
	else if (strcmp(argv[1], "plane") == 0){
		if (argc < 4){
			cout << "Too few arguments";
			return 0;
		}
		cout << "Generating Plane... \n";
		writePlane(atoi(argv[2]), atoi(argv[3]));
		cout << "Done \n";
	}
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
	}
	else
		cout << "Command not recognized";
}

