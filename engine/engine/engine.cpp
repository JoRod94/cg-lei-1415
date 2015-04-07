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

//Menu variables
GLenum mode = GL_FILL;
bool shouldDrawGrid = false;

//Camera variables
bool freeCamera = false;
bool keyHolds[256];
float alpha = 0;
float beta = 0;
float radius = 10;
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float rx = 0.0f;
float ry = 0.0f;
float rz = 0.0f;
int xOri = -1;
int yOri = -1;

void keyHoldsInit(){
	for (int i = 0; i < 256; i++)
		keyHolds[i] = false;
}

void keyActions(){
	if (keyHolds['z'] == true)
		radius--;
	if (keyHolds['x'] == true)
		radius++;
	if (keyHolds['w'] == true){
		pz += 0.01f*rz;
		px += 0.01f*rx;
		py += 0.01f*ry;
	}
	if (keyHolds['s'] == true){
		pz -= 0.01f*rz;
		px -= 0.01f*rx;
		py -= 0.01f*ry;
	}
	if (keyHolds['d'] == true){
		pz += 0.01f*rx;
		px -= 0.01f*rz;
	}
	if (keyHolds['a'] == true){
		pz -= 0.01f*rx;
		px += 0.01f*rz;
	}
}

void drawGrid(){
	if (shouldDrawGrid){
		glColor3ub(0, 255, 0);

		for (float i = -50; i <= 50; i += 1){
			glBegin(GL_LINES);

			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);

			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);

			glEnd();
		}
	}
}

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

	points.resize(arraySize);
	i.read((char *)&points[0], arraySize*sizeof(point));

	files[filename] = figure{ points, 1 };
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
	keyActions();
	if (freeCamera){
		gluLookAt(px, py, pz,
			px + rx, py + ry, pz + rz,
			0.0f, 1.0f, 0.0f);
	}
	else{
		gluLookAt((radius*(cos(beta))*(sin(alpha))), radius*(sin(beta)), (radius*(cos(beta))*(cos(alpha))),
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);
	}
	glColor3f(1, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, mode);

	renderPoints();

	drawGrid();

	glutSwapBuffers();
}


void keyBoardInput(unsigned char key, int x, int y){
	if (key == 'f'){
		alpha = 0.0f;
		beta = 0.0f;
		freeCamera = !freeCamera;
	}
	else
		keyHolds[key] = true;

	glutPostRedisplay();
}

void keyUp(unsigned char key, int x, int y){
	keyHolds[key] = false;
	glutPostRedisplay();
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

		alpha -= xDiff * 0.01f;
		beta -= yDiff * 0.01f;

		if (beta > (M_PI / 2) - 0.001) beta = (M_PI / 2) - 0.001;
		if (beta < -(M_PI / 2) + 0.001) beta = -(M_PI / 2) + 0.001;

		rx = radius*cos(beta)*sin(alpha);
		ry = radius*sin(beta);
		rz = radius*cos(beta)*cos(alpha);

	}
	xOri = x;
	yOri = y;
	glutPostRedisplay();
}


void polygonModeHandler(int id_op){
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

void gridModeHandler(int id_op){
	switch (id_op){
		case 1:
			shouldDrawGrid = false;
			break;
		case 2:
			shouldDrawGrid = true;
			break;
	}
}

void mainMenuHandler(int id_op){}

void createMenu(){
	int polygonMode, gridMode, mainMenu;

	polygonMode = glutCreateMenu(polygonModeHandler);
	glutAddMenuEntry("Line Mode", 1);
	glutAddMenuEntry("Point Mode", 2);
	glutAddMenuEntry("Fill Mode", 3);

	gridMode = glutCreateMenu(gridModeHandler);
	glutAddMenuEntry("No Grid", 1);
	glutAddMenuEntry("X-Z Grid", 2);

	mainMenu = glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Polygon Mode", polygonMode);
	glutAddSubMenu("Grid Mode", gridMode);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
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

	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("TP");


	// registo de funções 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	// pôr aqui a criação do menu
	createMenu();

	// Camera stuff
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyBoardInput);
	glutKeyboardUpFunc(keyUp);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// initialize keyHolds array
	keyHoldsInit();

	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 1;
}
