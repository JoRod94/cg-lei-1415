// engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "point.h"
#include "tinyxml2.h"

#define _XML_FILE	"ficheiro"
#define _XML_SCENE	"cena"
#define _XML_MODEL	"modelo"

using namespace std;

point *points;

void read_bin(string filename){
	int arraySize;

	ifstream i(filename, ios::binary);
	i.read((char *)&arraySize, sizeof(arraySize));

	points = (point *) malloc(arraySize*sizeof(point));

	i.read( (char *) points, arraySize*sizeof(point));
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

void parseScene(tinyxml2::XMLElement* scene) {
	for (tinyxml2::XMLElement* model = scene->FirstChildElement(_XML_MODEL); model != NULL; model = model->NextSiblingElement(_XML_MODEL))
		cout << model->Attribute(_XML_FILE) << endl;
		// read_file(model -> Attribute("ficheiro");
}

void read_xml(char* filename) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(filename);

	for (tinyxml2::XMLElement* scene = doc.FirstChildElement(_XML_SCENE); scene != NULL; scene = scene->NextSiblingElement(_XML_SCENE))
		parseScene(scene);
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Hello world! This is engine!" << endl;
	// read_file("test.txt");
	read_xml("model.xml");
	return 0;
}

