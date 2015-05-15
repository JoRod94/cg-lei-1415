#ifndef _CANVAS_H
#define _CANVAS_H

#include "transformation.h"
#include "color.h"
#include <vector>
#include <string>
#include <pair>

#define LIGHT_VECTOR    0
#define LIGHT_POINT     1

typedef struct s_figure {
    float* vertex;
    float* normal;
    unsigned int* indices;
    unsigned int n_ind;
    unsigned int n_coords;
    int buffer_nr;
} *figure;

typedef struct s_group {
    std::vector<Transformation*> transformations;
    std::vector< std::pair<string, Color> > points;
    std::vector<struct s_group*> subgroups;
} *group;

typedef struct s_light {
   int type;
   int x;
   int y;
   int z;
} *light;

typedef struct s_scene {
    std::vector<light> lights;
    std::vector<group> groups;
} *scene;

scene new_scene();

light new_empty_light();

light new_light(int type, int x, int y, int z);

group new_empty_group();

group new_group(vector<Transformation*> transformations, vector<string> points, vector<group> subgroups);

figure new_figure();

#endif

