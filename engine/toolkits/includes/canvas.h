#ifndef _CANVAS_H
#define _CANVAS_H

#include "transformation.h"
#include "color.h"
#include <vector>
#include <string>
#include <utility>
#include <GL/glut.h>
#include "model.h"

using namespace std;

typedef struct s_light {
	GLenum lId;
	float* pos;
} *light;

typedef struct s_figure {
	float* vertex;
	float* normal;
	float* tex_coord;
	unsigned int* indices;
	unsigned int n_coords;
	unsigned int n_tex_coords;
	unsigned int n_ind;
	unsigned int bound_sphere_Radius;
	int vertex_buffer_nr;
	int normal_buffer_nr;
	int texture_buffer_nr;
	bool has_tex;
} *figure;

typedef struct s_group {
	vector<Transformation*> transformations;
	vector<Model> points;
	vector<struct s_group *> subgroups;
} *group;

typedef struct s_scene {
    vector<light> lights;
    vector<group> groups;
} *scene;


scene new_scene(vector<group> groups, vector<light> lights);

light new_empty_light();

light new_light(GLenum lId, float x, float y, float z, float type);

group new_empty_group();

group new_group(vector<Transformation*> transformations, vector<Model> points, vector<group> subgroups);

figure new_figure();

#endif
