#ifndef _CANVAS_H
#define _CANVAS_H

#include "transformation.h"
#include "color.h"
#include <vector>
#include <string>
#include <utility>

using namespace std;

enum LightType { LIGHT_POINT, LIGHT_VECTOR };

typedef struct s_light {
   LightType type;
   int x;
   int y;
   int z;
} *light;

typedef struct s_figure {
	float* vertex;
	float* normal;
	unsigned int* indices;
	unsigned int n_ind;
	unsigned int n_coords;
	int vertex_buffer_nr;
	int normal_buffer_nr;
} *figure;

typedef struct s_group {
	vector<Transformation*> transformations;
	vector<pair<string, Color*> > points;
	vector<struct s_group *> subgroups;
} *group;

typedef struct s_scene {
    vector<light> lights;
    vector<group> groups;
} *scene;

scene new_scene(vector<group> groups, vector<light> lights);

light new_empty_light();

light new_light(LightType type, int x, int y, int z);

group new_empty_group();

group new_group(vector<Transformation*> transformations, vector<pair<string, Color*> > points, vector<group> subgroups);

figure new_figure();

#endif