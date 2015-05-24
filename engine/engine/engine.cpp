// engine.cpp : Defines the entry point for the console application.
#pragma comment(lib,"glew32.lib")
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <GL/glut.h>
#include <regex>
#include <utility>
#include "transformation.h"
#include "translation.h"
#include "color.h"
#include "rotation.h"
#include "scale.h"
#include "xml_reader.h"
#include "canvas.h"
#include "point.h"

#define DEFAULT_CAM_RADIUS	10.0f

using namespace std;

// VBO variables
GLuint *buffers;

vector<scene> scenes;
map<string, figure> files;

char* xmlName;

//Lighting variables
GLfloat amb[3] = { 0.2, 0.2, 0.2 };
GLfloat diff[3] = { 1.0, 1.0, 1.0 };

// Menu variables
GLenum mode = GL_FILL;
float gridSize = 50, gridScale = 1;
bool gridBools[4] = { false, false, false, false }; //shouldDrawGrid, drawXZ, drawXY, drawZY
float fps = 0.0f;
bool showFPS = false;
bool shouldDrawNormals = false;

// Time variables
int globalTime = 0;
int lastShowFPS = 0, fpsStep = 1000, frame = 0;
int lastRender = 0, renderStep = 7;

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

bool changed_color = false; // wether or not we have changed the color in a drawing iteration

void set_camera(float a, float b, float r) {
	defAlpha = (a * M_PI) / 180;
	defBeta = (b * M_PI) / 180;
    defRadius = r < 1 ? DEFAULT_CAM_RADIUS : r;
	alpha = (a * M_PI) / 180;
	beta = (b * M_PI) / 180;
	radius = defRadius;
}

static void keyHoldsInit(){
	for (int i = 0; i < 256; i++)
		keyHolds[i] = false;
}

static void reset_color() {
	if (!changed_color)
		return;

	if (glIsEnabled(GL_LIGHTING)) {
		float wht[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wht);
	}
	else
		glColor3f(1.0f, 1.0f, 1.0f);

	changed_color = false;
}

void change_color() {
	changed_color = true;
}

static void keyActions(){
	if (freeCamera){
		if (keyHolds['w']) {
			pz += rz*freeCamSpeed;
			px += rx*freeCamSpeed;
			py += ry*freeCamSpeed;
		}
		if (keyHolds['s']) {
			pz -= rz*freeCamSpeed;
			px -= rx*freeCamSpeed;
			py -= ry*freeCamSpeed;
		}
		if (keyHolds['d']) {
			pz += rx*freeCamSpeed;
			px -= rz*freeCamSpeed;
		}
		if (keyHolds['a']) {
			pz -= rx*freeCamSpeed;
			px += rz*freeCamSpeed;
		}
	}

    else {
		if (keyHolds['z'] && radius > 1)
			radius--;
		if (keyHolds['x'])
			radius++;
	}
}

static void gridBoolInit(){
	for (int i = 0; i<4; i++)
		gridBools[i] = false;
}

static void drawGrid(){
	if (gridBools[0]){
		
		if (glIsEnabled(GL_LIGHTING)) {
			float green[4] = { 0.0f, 255.0f, 0.0f, 1.0f };
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
		}
		else
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

float fix_sign(float d1,float d2){
		return ((d1 <= 0) == (d2 <= 0))?d2:-(d2);
}

static void draw_vbo(figure f){

	if (shouldDrawNormals){
		glBegin(GL_LINES);
		for (int i = 0; i < f->n_coords; i += 3){
			glVertex3f(f->vertex[i], f->vertex[i + 1], f->vertex[i + 2]);
			glVertex3f(f->vertex[i] + fix_sign(f->vertex[i], f->normal[i]),
				f->vertex[i + 1] + fix_sign(f->vertex[i + 1], f->normal[i + 1]),
				f->vertex[i + 2] + fix_sign(f->vertex[i + 2], f->normal[i + 2]));
		}
		glEnd();
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffers[f->vertex_buffer_nr]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[f->normal_buffer_nr]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLES, f->n_ind, GL_UNSIGNED_INT, f->indices);
}

static void draw_group(group g) {
	glPushMatrix();

	for (unsigned int i = 0; i < g->transformations.size(); i++) {
		(g->transformations[i])->apply();
	}

	for (unsigned int i = 0; i < g->points.size(); i++) {
		map<string, figure>::iterator p = files.find((g->points)[i].first);
		Color* c = (g->points)[i].second;
		if (c != nullptr)
			c->apply();
		if (p != files.end()) {
			draw_vbo(p->second);
		}
    }

	for (unsigned int i = 0; i < g->subgroups.size(); i++) {
		draw_group(g->subgroups[i]);
	}

    glPopMatrix();
	
}


static void renderPoints(vector<group> groups) {
	for (vector<group>::iterator it = groups.begin();
		it != groups.end();
		++it){
		reset_color();
		draw_group(*it);
	}
}

static void renderLights(vector<light> lights) {
	for (int i = 0; i < lights.size(); i++){
		glLightfv(lights[i]->lId, GL_POSITION, lights[i]->pos);
	}
}

static void renderScenes() {
	vector<scene>::iterator i = scenes.begin();

	while (i != scenes.end()) {
		renderLights((*i)->lights);
		renderPoints((*i)->groups);
		i++;
	}
}

static void generate_vbos(){
	int size = 2 * files.size();
	map<string, figure>::iterator fIt = files.begin();
	buffers = (GLuint *) malloc(size * (sizeof(GLuint)) );


	glGenBuffers(size, buffers);

	for (int i = 0; fIt != files.end(); fIt++) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[fIt->second->vertex_buffer_nr]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (fIt->second->n_coords), fIt->second->vertex, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[fIt->second->normal_buffer_nr]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (fIt->second->n_coords), fIt->second->normal, GL_STATIC_DRAW);
	}
}

void create_lights() {
	int size;
	vector<light> vl;
	bool enabled = false;
	for (int i = 0; i < scenes.size(); i++) {
		vl = scenes[i]->lights;
		size = vl.size();
		for (int j = 0; j < size; j++){
			glLightfv(vl[j]->lId, GL_AMBIENT, amb);
			glLightfv(vl[j]->lId, GL_DIFFUSE, diff);
			glEnable(vl[j]->lId);
			if(!enabled) enabled = true;
		}
	}

	if(enabled)
		glEnable(GL_LIGHTING);
}

static void changeSize(int w, int h) {

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

static void renderScene(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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


	glPolygonMode(GL_FRONT, mode);

	drawGrid();
	renderScenes();

	glutSwapBuffers();
}

static void renderTimer(){
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


static void keyBoardInput(unsigned char key, int x, int y){
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

static void keyUp(unsigned char key, int x, int y){
	keyHolds[key] = false;
}

// Mouse button callback
static void mousePress(int button, int state, int x, int y) {

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
static void mouseMotion(int x, int y) {

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

static void reset_engine() {
	glDisable(GL_LIGHTING);

	for (int i = 0; i < scenes.size(); i++) {
		vector<light> vl = scenes[i]->lights;
		int size = vl.size();
		for (int j = 0; j < size; j++){
			glDisable(vl[j]->lId);
		}
	}

	scenes.clear();
	files.clear();
}

static void reload_engine() {
	pair<vector<scene>, map<string, figure> > read_values = reset_and_read_xml(xmlName);

	scenes = read_values.first;
	files = read_values.second;

	create_lights();
	generate_vbos();

	glutPostRedisplay();
}

static void polygonModeHandler(int id_op){
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

static void gridModeHandler(int id_op){
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

static void toggleHandler(int id_op){
	switch (id_op) {
	case 1:
		showFPS = !showFPS;
		glutSetWindowTitle("Motor 3D");
		break;

	case 2:
		shouldDrawNormals = !shouldDrawNormals;
		break;

	case 3:
		for (int i = 0; i < scenes.size(); i++) {
			vector<group> groups = scenes[i]->groups;
			for (int j = 0; j < groups.size(); j++) {
				vector<Transformation*> vt = groups[j]->transformations;
				for (int w = 0; w < vt.size(); w++) {
					if (Translation* t = dynamic_cast<Translation*>(vt[w]))
						t->toggle_line();
				}
			}
		}

		break;
	}
}

static void mainMenuHandler(int id_op) {
	if (id_op == 4){
		reset_engine();
		reload_engine();
	}

}

static void createMenu(){
	int polygonMode, gridMode, toggle, mainMenu;

	polygonMode = glutCreateMenu(polygonModeHandler);
	glutAddMenuEntry("Line Mode", 1);
	glutAddMenuEntry("Point Mode", 2);
	glutAddMenuEntry("Fill Mode", 3);

	gridMode = glutCreateMenu(gridModeHandler);
	glutAddMenuEntry("No Grid", 1);
	glutAddMenuEntry("X-Z Grid", 2);
	glutAddMenuEntry("X-Y Grid", 3);
	glutAddMenuEntry("Z-Y Grid", 4);

	toggle = glutCreateMenu(toggleHandler);
	glutAddMenuEntry("Toggle FPS Counter", 1);
	glutAddMenuEntry("Toggle Model Normals", 2);
	glutAddMenuEntry("Toggle Curve Lines", 3);

	mainMenu = glutCreateMenu(mainMenuHandler);
	glutAddSubMenu("Polygon Mode", polygonMode);
	glutAddSubMenu("Grid Mode", gridMode);
	glutAddSubMenu("Toggle", toggle);
	glutAddMenuEntry("Reload XML", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

static int valid_xml(char* filename) {
	int name = regex_match(filename, regex(".*\.xml"));

	ifstream f(filename);
	int exists = f.good();
	f.close();

	if (name && exists)
		return name;
}

static void initGL(){
	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);

	// pôr aqui a criação do menu
	createMenu();

	// initialize keyHolds array
	keyHoldsInit();

	//VBOs
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	create_lights();

	generate_vbos();
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
	pair<vector<scene>, map<string, figure> > read_values = read_xml(xmlName);
    scenes = read_values.first;
    files = read_values.second;

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

	//gl
	initGL();

	// entrar no ciclo do GLUT
	glutMainLoop();

	return 1;
}
