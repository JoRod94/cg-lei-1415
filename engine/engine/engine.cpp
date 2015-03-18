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
#include <GL/glut.h>
#include <regex>
#define _USE_MATH_DEFINES
#include <math.h>

#define _XML_FILE	"ficheiro"
#define _XML_SCENE	"cena"
#define _XML_MODEL	"modelo"

using namespace std;

typedef struct figure {
	vector<point> points;
	int count;
} figure;

map<string, figure> files;
vector<point> points;
GLenum mode = GL_FILL;

// Camera variables
float alpha = 0;
float beta = 0;
float radius = 10;
int xOri = -1;
int yOri = -1;


void renderPoints(){

	map<string, figure>::iterator iter;
	glBegin(GL_TRIANGLES);
	for (iter = files.begin(); iter != files.end(); iter++) {
		for (int j = 0; j < iter->second.count; j++)
			for (int i = 0; i < (iter->second).points.size(); i++)
				glVertex3f((iter->second).points[i].x, (iter->second).points[i].y, (iter->second).points[i].z);
	}
	glEnd();
}

void read_bin(string filename){
	unsigned long int arraySize;

	map<string, figure>::iterator file = files.find(filename);

	if (file != files.end()) {
		points = file->second.points;
		file->second.count++;
		cout << "FILE ALREADY EXISTS" << endl;
		return;
	}

	ifstream i(filename, ios::binary);
	if (!i) {
		cout << "INEXISTENT FILE" << endl;
		return;
	}

	i.read((char *)&arraySize, sizeof(arraySize));

	cout << "\n########## READING BINARY FILE ##########"
		<< "\n\nNR POINTS:"
		<< arraySize
		<< endl;

	points.resize(arraySize);
	i.read((char *)&points[0], arraySize*sizeof(point));

	cout << "\n########## FINISHED FILE READ ##########" << endl;

	cout << "\nINSERTING INTO HASH...\n"
		<< endl;

	files[filename] = figure{points, 1};

	cout << "\nFINISHED INSERTING INTO HASH" << endl;
}


void parseScene(tinyxml2::XMLElement* scene) {
	for (tinyxml2::XMLElement* model = scene->FirstChildElement(_XML_MODEL); model != NULL; model = model->NextSiblingElement(_XML_MODEL))
		read_bin(model->Attribute(_XML_FILE));
}


void read_xml(char * xmlName) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlName);

	for (tinyxml2::XMLElement* scene = doc.FirstChildElement(_XML_SCENE); scene != NULL; scene = scene->NextSiblingElement(_XML_SCENE))
		parseScene(scene);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	//TESTE, ALTERAR
	gluLookAt((radius*(cos(beta))*(sin(alpha))), radius*(sin(beta)), (radius*(cos(beta))*(cos(alpha))),
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	glColor3f(1, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, mode);

	renderPoints();

	glutSwapBuffers();
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


// Mouse button callback
void mousePress(int button, int state, int x, int y) {

	// Camera only moves while the left mouse button is pressed
	if (button == GLUT_LEFT_BUTTON) {
		// If the button is released, origin coordinates reset.
		if (state == GLUT_UP) {
			xOri = -1;
			yOri = -1;
		}
		// Else, button is pressed, origin coordinates are updated.
		else  {
			xOri = x;
			yOri = y;
		}
		glutPostRedisplay();
	}
}


// Motion while mouse button is pressed
void mouseMotion(int x, int y) {

	if (xOri >= 0) {
		int xDiff = x - xOri;
		int yDiff = y - yOri;

		alpha += xDiff * 0.01f;

		if (yDiff > 0){
			if (beta <= (M_PI / 2))
				beta += yDiff * 0.01f;
		}
		else
			if (beta >= -(M_PI / 2))
				beta -= -(yDiff * 0.01f);
	}
	xOri = x;
	yOri = y;
	glutPostRedisplay();
}


void createMenu(int id_op){
	switch (id_op){
	case 1:
		mode = GL_LINE;
		break;
	case 2:
		mode = GL_POINT;
		break;
	case 3:
		mode = GL_FILL;
		break;
	}
	glutPostRedisplay();
}

int valid_xml(char* filename) {
	int name = regex_match(filename, regex(".*\.xml"));
	ifstream f(filename);
	int exists = f.good();
	f.close();
	return name && exists;
}

int main(int argc, char **argv)
{
	if (argc < 2){
		cout << "Not enough arguments" << endl;
		return 0;
	}
	else if (argc > 2) {
		cout << "Too many arguments" << endl;
		return 0;
	}
	else if (!valid_xml(argv[1])) {
		cout << "Please enter a valid XML file" << endl;
		return 0;
	}

	read_xml(argv[1]);

	// inicializa��o
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("TP");


	// registo de fun��es 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// p�r aqui a cria��o do menu
	glutCreateMenu(createMenu);
	glutAddMenuEntry("Line Mode", 1);
	glutAddMenuEntry("Point Mode", 2);
	glutAddMenuEntry("Fill Mode", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Camera stuff
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMotion);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 1;
}
