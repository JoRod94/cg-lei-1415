// engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "point.h"
#include "tinyxml2.h"

#define _XML_FILE	"ficheiro"
#define _XML_SCENE	"cena"
#define _XML_MODEL	"modelo"

using namespace std;

map<string, vector<point>> files;
vector<point> points;

void read_bin(string filename){
	unsigned long int arraySize;

	map<string, vector<point>>::iterator file = files.find(filename);

	if (file != files.end()) {
		points = file->second;
		cout << "FILE ALREADY EXISTS" << endl;
		return;
	}

	ifstream i(filename, ios::binary);
	i.read((char *)&arraySize, sizeof(arraySize));
	
	cout << "\n########## READING BINARY FILE ##########"
		<< "\n\nNR POINTS:"
		<< arraySize
		<< endl;

	points.resize(arraySize);
	i.read( (char *) &points[0], arraySize*sizeof(point));

	cout << "\n########## FINISHED FILE READ ##########" << endl;

	cout << "\nINSERTING INTO HASH...\n"
		<< endl;

	files[filename] = points;

	cout << "\nFINISHED INSERTING INTO HASH" << endl;
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
	read_bin("testing3.binary");
	// read_xml("model.xml");
	cout << "\n########## PRINTING VECTOR READ FROM FILE ##########" << endl;
	for (int i = 0; i < points.size(); i++){
		printf("X: %f, Y: %f, Z: %f\n", points[i].x, points[i].y, points[i].z);
	}
	cout << "\n########## FINISHED VECTOR PRINT ##########" << endl;
	
	cout << "\n\n\n\n\n SECOND READ" << endl;
	read_bin("testing3.binary");
	// read_xml("model.xml");
	cout << "\n########## PRINTING VECTOR READ FROM FILE ##########" << endl;
	for (int i = 0; i < points.size(); i++){
		printf("X: %f, Y: %f, Z: %f\n", points[i].x, points[i].y, points[i].z);
	}
	cout << "\n########## FINISHED VECTOR PRINT ##########" << endl;



	return 0;
}

