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
#include "transformation.h"
#include "translation.h"
#include "color.h"
#include "rotation.h"
#include "scale.h"
#define _USE_MATH_DEFINES
#include <math.h>


#define _XML_FILE           "ficheiro"
#define _XML_SCENE          "cena"
#define _XML_CAMERA			"camera"
#define _XML_MODEL          "modelo"
#define _XML_MODELS         "modelos"
#define _XML_GROUP          "grupo"
#define _XML_TRANSLATION    "translacao"
#define _XML_COLOR			"cor"
#define _XML_ROTATION       "rotacao"
#define _XML_SCALE          "escala"
#define _XML_X              "X"
#define _XML_Y              "Y"
#define _XML_Z              "Z"
#define _XML_R              "R"
#define _XML_G              "G"
#define _XML_B              "B"
#define _XML_ANGLE          "angulo"
#define _XML_X_AXIS         "eixoX"
#define _XML_Y_AXIS         "eixoY"
#define _XML_Z_AXIS         "eixoZ"
#define _XML_CAM_RADIUS		"raio"
#define _XML_CAM_ALPHA		"alfa"
#define _XML_CAM_BETA		"beta"

using namespace std;

typedef struct s_group {
    vector<Transformation*> transformations;
    vector<string> points;
    vector<struct s_group *> subgroups;
} *group;

vector<group> groups;
map<string, vector<point>> files;

//Menu variables

GLenum mode = GL_FILL;
bool shouldDrawGrid = false;

//Camera variables
bool freeCamera = false;
bool keyHolds[256];
float alpha = 0, freeAlpha = 0;
float beta = 0, freeBeta = 0;
float radius = 1;
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float rx = 0.0f;
float ry = 0.0f;
float rz = 0.0f;
int xOri = -1;
int yOri = -1;
bool camera_set = false;

void set_camera(float a, float b, float r) {
	if (!camera_set) {
		alpha = a;
		beta = b;
		radius = r || 1;
		camera_set = true;
	}
}

void keyHoldsInit(){
	for (int i = 0; i < 256; i++)
		keyHolds[i] = false;
}

void keyActions(){
	if (keyHolds['z'])
		radius--;
	if (keyHolds['x'])
		radius++;
	if (keyHolds['w']){
		pz += 0.01f*rz;
		px += 0.01f*rx;
		py += 0.01f*ry;
	}
	if (keyHolds['s']){
		pz -= 0.01f*rz;
		px -= 0.01f*rx;
		py -= 0.01f*ry;
	}
	if (keyHolds['d']){
		pz += 0.01f*rx;
		px -= 0.01f*rz;
	}
	if (keyHolds['a']){
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

group new_group(vector<Transformation*> transformations, vector<string> points, vector<group> subgroups) {
	group g = (group)calloc(1, sizeof(struct s_group));

	g->transformations = transformations;

	g->points = points;

    	g -> subgroups = subgroups;

	return g;
}


void draw_points(vector<point> vp){
	glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < vp.size(); i++) {
		glVertex3f(vp[i].x, vp[i].y, vp[i].z);
	}

	glEnd();
}

void draw_group(group g) {
	glPushMatrix();

	for (unsigned int i = 0; i < g->transformations.size(); i++) {
		(g->transformations[i])->apply();
	}


	for (unsigned int i = 0; i < g->points.size(); i++) {
        		map<string, vector<point>>::iterator p = files.find(g->points[i]);

		if (p != files.end()) {
			draw_points(p->second);
		}
    }

	for (unsigned int i = 0; i < g->subgroups.size(); i++) {
		draw_group(g->subgroups[i]);
	}

    glPopMatrix();
}


void renderPoints() {
	for (vector<group>::iterator it = groups.begin();
		it != groups.end();
		++it)
		draw_group(*it);
}

void read_bin(string filename){
	unsigned long int arraySize;
	vector<point> points;

	map<string, vector<point>>::iterator file = files.find(filename);

	if (file != files.end()) {
		points = file->second;
		cout << "MODEL FILE ALREADY READ: " << filename << endl;
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

	files[filename] = points;
}

static bool valid_group(tinyxml2::XMLElement* group) {
    tinyxml2::XMLElement* models = group->FirstChildElement(_XML_MODELS);
    tinyxml2::XMLElement* translation = group->FirstChildElement(_XML_TRANSLATION);
    tinyxml2::XMLElement* rotation = group->FirstChildElement(_XML_ROTATION);
    tinyxml2::XMLElement* scale = group->FirstChildElement(_XML_SCALE);
	tinyxml2::XMLElement* color = group->FirstChildElement(_XML_COLOR);

    return (
            (models == NULL || models->NextSiblingElement(_XML_MODELS) == NULL) &&
            (translation == NULL || translation->NextSiblingElement(_XML_TRANSLATION) == NULL) &&
            (rotation == NULL || rotation->NextSiblingElement(_XML_ROTATION) == NULL) &&
            (scale == NULL || scale->NextSiblingElement(_XML_SCALE) == NULL) &&
			(color == NULL || color->NextSiblingElement(_XML_COLOR) == NULL)
           );
}

static vector<string> group_points(tinyxml2::XMLElement* group) {
    vector<string> points;
    tinyxml2::XMLElement* models = group->FirstChildElement(_XML_MODELS);
	
	if (models) {
		for (tinyxml2::XMLElement* model = models->FirstChildElement(_XML_MODEL);
			model != NULL; model = model->NextSiblingElement(_XML_MODEL)) {
			string filename = model->Attribute(_XML_FILE);
			read_bin(filename);
			points.push_back(filename);
		}
	}

    return points;
}

static vector<Transformation*> group_colors(tinyxml2::XMLElement* g) {
	vector<Transformation*> v;
	tinyxml2::XMLElement* color = g->FirstChildElement(_XML_COLOR);

	while (color != NULL) {
		v.push_back(new Color(
			color->FloatAttribute(_XML_R),
			color->FloatAttribute(_XML_G),
			color->FloatAttribute(_XML_B)
			));

		color = color->NextSiblingElement(_XML_COLOR);
	}

	return v;
}

static vector<Transformation*> group_transformations(tinyxml2::XMLElement* group) {
    vector<Transformation*> vt;

    for(tinyxml2::XMLElement* translation = group->FirstChildElement(_XML_TRANSLATION);
            translation != NULL; translation = translation->NextSiblingElement(_XML_TRANSLATION)) {
		vt.push_back( new Translation(
                    translation->FloatAttribute(_XML_X),
                    translation->FloatAttribute(_XML_Y),
                    translation->FloatAttribute(_XML_Z) ));
    }

    for(tinyxml2::XMLElement* rotation = group->FirstChildElement(_XML_ROTATION);
            rotation != NULL; rotation = rotation->NextSiblingElement(_XML_ROTATION)) {
		vt.push_back( new Rotation(
                    rotation->FloatAttribute(_XML_ANGLE),
                    rotation->FloatAttribute(_XML_X_AXIS),
                    rotation->FloatAttribute(_XML_Y_AXIS),
                    rotation->FloatAttribute(_XML_Z_AXIS) ));
    }

    for(tinyxml2::XMLElement* scale = group->FirstChildElement(_XML_SCALE);
            scale != NULL; scale = scale->NextSiblingElement(_XML_SCALE)) {
        vt.push_back( new Scale(
                    scale->FloatAttribute(_XML_X),
                    scale->FloatAttribute(_XML_Y),
                    scale->FloatAttribute(_XML_Z) ) );
    }

    return vt;
}

vector<Transformation*> colorize(tinyxml2::XMLElement* g) {
	vector<Transformation*> v;

	tinyxml2::XMLElement* color = g->FirstChildElement(_XML_COLOR);
	if (color == NULL)
		v.push_back(new Color(255, 255, 255));
	else
		v = group_colors(g);

	return v;
}

bool __parse_group(tinyxml2::XMLElement* g, group *ret) {
    if(! valid_group(g)) {
        cout << "Invalid group found. Ignoring..." << endl;
        return false;
    }

    vector<Transformation*> t = group_transformations(g);
	vector<Transformation*> c = group_colors(g);
    vector<string> pt = group_points(g);
    vector<group> sg;

	t.insert(t.end(), c.begin(), c.end());

	tinyxml2::XMLElement* subgroup = g->FirstChildElement(_XML_GROUP);
	while(subgroup != NULL) {
			group maybe_sub = (group)malloc(sizeof(struct s_group));

			if (__parse_group(subgroup, &maybe_sub)) {
				sg.push_back(maybe_sub);
			}
			subgroup = subgroup->NextSiblingElement(_XML_GROUP);
       }

    *ret = new_group(t, pt, sg);
    return true;
}

bool parseGroup(tinyxml2::XMLElement* g, group *ret) {
	vector<Transformation*> colors = colorize(g);
	bool r = __parse_group(g, ret);
	if (r)
		(*ret)->transformations.insert((*ret)->transformations.end(), colors.begin(), colors.end());
	return r;
}

void read_xml(char* xmlName) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlName);
	group ret = (group)malloc(sizeof(struct s_group));

	for (tinyxml2::XMLElement* scene = doc.FirstChildElement(_XML_SCENE);
			scene != NULL; scene = scene->NextSiblingElement(_XML_SCENE)) {

		tinyxml2::XMLElement* camera = scene->FirstChildElement(_XML_CAMERA);
		if (camera)
			set_camera(camera->FloatAttribute(_XML_CAM_ALPHA),
						camera->FloatAttribute(_XML_CAM_BETA),
						camera->FloatAttribute(_XML_CAM_RADIUS));

		for (tinyxml2::XMLElement* g = scene->FirstChildElement(_XML_GROUP);
				g != NULL; g = g->NextSiblingElement(_XML_GROUP))
		{
			if ( parseGroup(g, &ret) ) {
				groups.push_back(ret);
			}
		}
	}
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
	if (key == 'f')
		freeCamera = !freeCamera;
	else if (key == 'r'){
		if (freeCamera){
			px = 0.0f;
			py = 0.0f;
			pz = 0.0f;
			freeAlpha = 0.0f;
			freeBeta = 0.0f;
		}
		else if (!freeCamera){
			radius = 10;
			alpha = 0.0f;
			beta = 0.0f;
		}
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

		if (freeCamera){
			freeAlpha -= xDiff * 0.01f;
			freeBeta -= yDiff * 0.01f;

			if (freeBeta > (M_PI / 2) - 0.001) freeBeta = (M_PI / 2) - 0.001;
			if (freeBeta < -(M_PI / 2) + 0.001) freeBeta = -(M_PI / 2) + 0.001;

			rx = radius*cos(freeBeta)*sin(freeAlpha);
			ry = radius*sin(freeBeta);
			rz = radius*cos(freeBeta)*cos(freeAlpha);
		}
		else{
			alpha -= xDiff * 0.01f;
			beta -= yDiff * 0.01f;

			if (beta > (M_PI / 2) - 0.001) beta = (M_PI / 2) - 0.001;
			if (beta < -(M_PI / 2) + 0.001) beta = -(M_PI / 2) + 0.001;
		}
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
