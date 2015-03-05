// generator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;

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

