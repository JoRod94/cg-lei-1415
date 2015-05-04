// engine.cpp : Defines the entry point for the console application.
#pragma comment(lib,"glew32.lib")
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <glew.h>
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
#define _XML_POINT			"ponto"
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
#define _XML_TIME			"tempo"
#define _XML_LINE			"linha"
#define _XML_CAM_RADIUS		"raio"
#define _XML_CAM_ALPHA		"alfa"
#define _XML_CAM_BETA		"beta"
#define DEFAULT_CAM_RADIUS	10.0f

using namespace std;

typedef struct s_group {
    vector<Transformation*> transformations;
    vector<string> points;
    vector<struct s_group *> subgroups;
} *group;

typedef struct s_figure {
	float* vertex;
	unsigned int* indices;
	unsigned int n_ind;
	unsigned int n_coords;
	int buffer_nr;
} *figure;

vector<group> groups;
map<string, figure> files;

char* xmlName;
int active_buffer = 0;

// Menu variables
GLenum mode = GL_FILL;
float gridSize = 50, gridScale = 1;
bool gridBools[4] = { false, false, false, false }; //shouldDrawGrid, drawXZ, drawXY, drawZY
float fps = 0.0f;
bool showFPS = false;

// Time variables
int globalTime = 0;
int lastShowFPS = 0, fpsStep = 1000, frame = 0;
int lastRender = 0, renderStep = 7;

// VBO variables
float *vertexB;
unsigned int *indices;
GLuint *buffers;

// Camera variables
bool freeCamera = false;
bool keyHolds[256];
float defAlpha = 0, defBeta = 0, defRadius = DEFAULT_CAM_RADIUS;
float alpha = 0, freeAlpha = 0;
float beta = 0, freeBeta = 0;
float radius = DEFAULT_CAM_RADIUS;
float freeCamSpeed = 0.5;
float px = 0.0f;
float py = 0.0f;
float pz = 0.0f;
float rx = 1.0f;
float ry = 0.0f;
float rz = 0.0f;
int xOri = -1;
int yOri = -1;

figure new_figure() {
	return (figure)malloc(sizeof(struct s_figure));
}

void set_camera(float a, float b, float r) {
	defAlpha = (a * M_PI) / 180;
	defBeta = (b * M_PI) / 180;
    defRadius = r < 1 ? DEFAULT_CAM_RADIUS : r;
	alpha = (a * M_PI) / 180;
	beta = (b * M_PI) / 180;
	radius = defRadius;
}

void keyHoldsInit(){
	for (int i = 0; i < 256; i++)
		keyHolds[i] = false;
}

void keyActions(){
	if (freeCamera){
		if (keyHolds['w']){
			pz += rz*freeCamSpeed;
			px += rx*freeCamSpeed;
			py += ry*freeCamSpeed;
		}
		if (keyHolds['s']){
			pz -= rz*freeCamSpeed;
			px -= rx*freeCamSpeed;
			py -= ry*freeCamSpeed;
		}
		if (keyHolds['d']){
			pz += rx*freeCamSpeed;
			px -= rz*freeCamSpeed;
		}
		if (keyHolds['a']){
			pz -= rx*freeCamSpeed;
			px += rz*freeCamSpeed;
		}
	}
	else{
		if (keyHolds['z'] && radius > 1)
			radius--;
		if (keyHolds['x'])
			radius++;
	}
}

void gridBoolInit(){
	for (int i = 0; i<4; i++)
		gridBools[i] = false;
}

void drawGrid(){
	if (gridBools[0]){
		glColor3ub(0, 255, 0);
		if (gridBools[1]){
			for (float i = -gridSize; i <= gridSize; i += gridScale){
				glBegin(GL_LINES);

				glVertex3f(-gridSize, 0, i);
				glVertex3f(gridSize, 0, i);

				glVertex3f(i, 0, -gridSize);
				glVertex3f(i, 0, gridSize);

				glEnd();
			}
		}
		if (gridBools[2]){
			for (float i = -gridSize; i <= gridSize; i += gridScale){
				glBegin(GL_LINES);

				glVertex3f(-gridSize, i, 0);
				glVertex3f(gridSize, i, 0);

				glVertex3f(i, -gridSize, 0);
				glVertex3f(i, gridSize, 0);

				glEnd();
			}
		}
		if (gridBools[3]){
			for (float i = -gridSize; i <= gridSize; i += gridScale){
				glBegin(GL_LINES);

				glVertex3f(0, -gridSize, i);
				glVertex3f(0, gridSize, i);

				glVertex3f(0, i, -gridSize);
				glVertex3f(0, i, gridSize);

				glEnd();
			}
		}
	}
}

group new_group(vector<Transformation*> transformations, vector<string> points, vector<group> subgroups) {
	group g = (group)calloc(1, sizeof(struct s_group));

	g->transformations = transformations;

	g->points = points;

    g->subgroups = subgroups;

	return g;
}

group new_empty_group() {
	group g = (group)calloc(1, sizeof(struct s_group));

	g->transformations = vector<Transformation*>();

	g->points = vector<string>();

	g->subgroups = vector<group>();

	return g;
}

void draw_vbo(figure f){
	glBindBuffer(GL_ARRAY_BUFFER, buffers[f->buffer_nr]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawElements(GL_TRIANGLES, f->n_ind, GL_UNSIGNED_INT, f->indices);
}


void draw_group(group g) {
	glPushMatrix();

	for (unsigned int i = 0; i < g->transformations.size(); i++) {
		(g->transformations[i])->apply();
	}


	for (unsigned int i = 0; i < g->points.size(); i++) {
        map<string, figure>::iterator p = files.find(g->points[i]);
		if (p != files.end()) {
			draw_vbo(p->second);
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
	figure f = new_figure();

	map<string, figure>::iterator file = files.find(filename);

	if (file != files.end()) {
		// f = file->second;
		free(f);
		cout << "MODEL FILE ALREADY READ: " << filename << endl;
		return;
	}

	ifstream i(filename, ios::binary);
	if (!i) {
		cout << "INEXISTENT FILE" << endl;
		return;
	}

	i.read((char *)&(f->n_coords), sizeof(unsigned int));
	f->vertex = (float *)malloc((f->n_coords)*sizeof(float));
	i.read((char *)f->vertex , f->n_coords*sizeof(float));

	i.read((char *)&(f->n_ind), sizeof(unsigned int));
	f->indices = (unsigned int *)malloc(f->n_ind*sizeof(unsigned int));
	i.read((char *)&(f->indices[0]), f->n_ind*sizeof(unsigned int));

	f->buffer_nr = active_buffer++;
	
	files[filename] = f;
}

void generate_vbos(){
	int size = files.size();
	map<string, figure>::iterator fIt = files.begin();
	buffers = (GLuint *) malloc(size * (sizeof(GLuint)) );


	glGenBuffers(size, buffers);

	for (int i = 0; fIt != files.end(); fIt++) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[fIt->second->buffer_nr]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (fIt->second->n_coords), fIt->second->vertex, GL_STATIC_DRAW);
	}

	free(vertexB);
}

static bool valid_translation(tinyxml2::XMLElement* t) {
	// check if we have an implicit point declarion in the translation tag
	bool no_implicit_point = t->Attribute(_XML_X) == NULL &&
								t->Attribute(_XML_Y) == NULL &&
								t->Attribute(_XML_Z) == NULL;

	tinyxml2::XMLElement* pt = t->FirstChildElement(_XML_POINT);

	// checking if we have explicit points definition
	bool no_explicit_points = pt == NULL;
	
	// checking if we have a valid nr of points
	int nr_points = 0;
	while (pt) {
		nr_points++;
		pt = pt->NextSiblingElement(_XML_POINT);
	}
	bool valid_points = nr_points >= 4;
	
	// this needs to be the only translation
	bool no_next_translation = t->NextSiblingElement(_XML_TRANSLATION) == NULL;

	// != is a logical exclusive or: either we have no implicit (atribute) point definition
	// or we have explicit point definition (point tag)
	// then, we can have no points definition, but if we have defined, they must be valid
	// this translation needs to be the only one as well
	return (no_implicit_point != no_explicit_points) && (no_explicit_points || valid_points) && no_next_translation;
}

static bool valid_group(tinyxml2::XMLElement* group) {
    tinyxml2::XMLElement* models = group->FirstChildElement(_XML_MODELS);
    tinyxml2::XMLElement* translation = group->FirstChildElement(_XML_TRANSLATION);
    tinyxml2::XMLElement* rotation = group->FirstChildElement(_XML_ROTATION);
    tinyxml2::XMLElement* scale = group->FirstChildElement(_XML_SCALE);
	tinyxml2::XMLElement* color = group->FirstChildElement(_XML_COLOR);

    return (
            (models == NULL || models->NextSiblingElement(_XML_MODELS) == NULL) &&
            (translation == NULL || valid_translation(translation)) &&
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

static void parse_translation(Translation* t, tinyxml2::XMLElement* node) {
	for (tinyxml2::XMLElement* p = node->FirstChildElement(_XML_POINT); p; p = p->NextSiblingElement(_XML_POINT))
		t->points.push_back(point(
									p->FloatAttribute(_XML_X),
									p->FloatAttribute(_XML_Y),
									p->FloatAttribute(_XML_Z)
								));
}

static vector<Transformation*> group_transformations(tinyxml2::XMLElement* group) {
    vector<Transformation*> vt;

	for (tinyxml2::XMLElement* node = group->FirstChildElement(); node; node = node->NextSiblingElement()) {
		string s = node->Value();

		if (s == _XML_ROTATION) {
			vt.push_back(new Rotation(
				node->FloatAttribute(_XML_TIME),
				node->FloatAttribute(_XML_ANGLE),
				node->FloatAttribute(_XML_X_AXIS),
				node->FloatAttribute(_XML_Y_AXIS),
				node->FloatAttribute(_XML_Z_AXIS)));
		}

		else if (s == _XML_TRANSLATION) {
			Translation* t = new Translation(
				node->FloatAttribute(_XML_TIME),
				node->FloatAttribute(_XML_X),
				node->FloatAttribute(_XML_Y),
				node->FloatAttribute(_XML_Z),
				node->BoolAttribute(_XML_LINE));
			parse_translation(t, node);
			vt.push_back(t);
		}

		else if (s == _XML_SCALE) {
			vt.push_back(new Scale(
				node->FloatAttribute(_XML_X),
				node->FloatAttribute(_XML_Y),
				node->FloatAttribute(_XML_Z)));
		}
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

bool __parse_group(tinyxml2::XMLElement* g, group &ret) {
    if(! valid_group(g)) {
        cout << "Invalid group found. Ignoring..." << endl;
        return false;
    }
    vector<Transformation*> t = group_transformations(g);
	vector<Transformation*> c = group_colors(g);
    vector<string> pt = group_points(g);
    vector<group> sg;
	
	for (int i = 0; i < c.size(); i++)
		t.push_back(c[i]);

	tinyxml2::XMLElement* subgroup = g->FirstChildElement(_XML_GROUP);
	while(subgroup != NULL) {
			group maybe_sub = (group)malloc(sizeof(struct s_group));


			if (__parse_group(subgroup, maybe_sub)) {
				sg.push_back(maybe_sub);
			}
			subgroup = subgroup->NextSiblingElement(_XML_GROUP);
       }

    ret = new_group(t, pt, sg);
    return true;
}

bool parseGroup(tinyxml2::XMLElement* g, group &ret) {
	vector<Transformation*> colors = colorize(g);
	bool r = __parse_group(g, ret);
	if (r) {
		if (ret->transformations.size() == 0)
			ret->transformations = colors;
		else {
			for (int i = 0; i < colors.size(); i++) {
				ret->transformations.push_back(colors[i]);
			}
		}
	}
	return r;
}

void read_xml() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlName);


    tinyxml2::XMLElement* camera = doc.FirstChildElement(_XML_CAMERA);
	if (camera){
		set_camera(camera->FloatAttribute(_XML_CAM_ALPHA),
			camera->FloatAttribute(_XML_CAM_BETA),
			camera->FloatAttribute(_XML_CAM_RADIUS));
	}

	group ret = new_empty_group();

    for (tinyxml2::XMLElement* scene = doc.FirstChildElement(_XML_SCENE);
			scene != NULL; scene = scene->NextSiblingElement(_XML_SCENE)) {
		for (tinyxml2::XMLElement* g = scene->FirstChildElement(_XML_GROUP);
				g != NULL; g = g->NextSiblingElement(_XML_GROUP))
		{
			if ( parseGroup(g, ret) ) {
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

void renderTimer(){
	globalTime = glutGet(GLUT_ELAPSED_TIME);

	if (globalTime - lastRender > renderStep ){
		frame++;
		lastRender = globalTime;
		glutPostRedisplay();
	}

	if (showFPS && (globalTime - lastShowFPS > fpsStep)) {
		char s[64];
		fps = frame*1000.0 / (globalTime - lastShowFPS);
		lastShowFPS = globalTime;
		frame = 0.0f;
		sprintf_s(s, "FPS: %f", fps);
		glutSetWindowTitle(s);
	}
}


void keyBoardInput(unsigned char key, int x, int y){
	if (gridBools[0]){
		if (key == 'c'){
			gridSize += gridSize;
			gridScale += gridScale;
		}
		if (key == 'v' && gridSize > 1){
			gridSize -= gridSize / 2;
			gridScale -= gridScale / 2;
		}
	}
	if (key == 'f')
		freeCamera = !freeCamera;
	else if (key == 'r'){
		if (freeCamera){
			px = 0.0f;
			py = 0.0f;
			pz = 0.0f;
			freeAlpha = defAlpha;
			freeBeta = defBeta;
		}
		else if (!freeCamera){
			alpha = defAlpha;
			beta = defBeta;
			radius = defRadius;
		}
	}
	else
		keyHolds[key] = true;
}

void keyUp(unsigned char key, int x, int y){
	keyHolds[key] = false;
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

			rx = cos(freeBeta)*sin(freeAlpha);
			ry = sin(freeBeta);
			rz = cos(freeBeta)*cos(freeAlpha);
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
	if (id_op == 1){
		gridBoolInit();
		gridSize = 50;
	}
	else{
		switch (id_op){
		case 2:
			gridBools[1] = true;
			break;
		case 3:
			gridBools[2] = true;
			break;
		case 4:
			gridBools[3] = true;
			break;
		}
		gridBools[0] = true;
	}
}

void mainMenuHandler(int id_op) {
	if (id_op == 3){
		showFPS = !showFPS;
		glutSetWindowTitle("Motor 3D");
	}
	if (id_op == 4) {
		groups.clear();
		files.clear();
		active_buffer = 0;
		read_xml();
		generate_vbos();
		glutPostRedisplay();
	}
}

void createMenu(){
	int polygonMode, gridMode, mainMenu;

	polygonMode = glutCreateMenu(polygonModeHandler);
	glutAddMenuEntry("Line Mode", 1);
	glutAddMenuEntry("Point Mode", 2);
	glutAddMenuEntry("Fill Mode", 3);

	gridMode = glutCreateMenu(gridModeHandler);
	glutAddMenuEntry("No Grid", 1);
	glutAddMenuEntry("X-Z Grid", 2);
	glutAddMenuEntry("X-Y Grid", 3);
	glutAddMenuEntry("Z-Y Grid", 4);

	mainMenu = glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Polygon Mode", polygonMode);
	glutAddSubMenu("Grid Mode", gridMode);
	glutAddMenuEntry("Toggle FPS Counter", 3);
	glutAddMenuEntry("Reload XML", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int valid_xml(char* filename) {
	int name = regex_match(filename, regex(".*\.xml"));

	ifstream f(filename);
	int exists = f.good();
	f.close();

	return name && exists;
}

int main(int argc, char **argv){
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


	xmlName = argv[1];
	read_xml();

	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Motor 3D");

	// registo de funções
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderTimer);
	glutReshapeFunc(changeSize);

	// Camera stuff
	glutMouseFunc(mousePress);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyBoardInput);
	glutKeyboardUpFunc(keyUp);

	//glew
	glewInit();

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// pôr aqui a criação do menu
	createMenu();


	// initialize keyHolds array
	keyHoldsInit();

	generate_vbos();

	// entrar no ciclo do GLUT
	glutMainLoop();

	return 1;
}
