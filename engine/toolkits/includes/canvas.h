#ifndef _CANVAS_H
#define _CANVAS_H

#include "transformation.h"
#include "color.h"
#include <vector>
#include <string>

#define LIGHT_VECTOR    0
#define LIGHT_POINT     1

using namespace std;

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
    std::vector< std::pair<std::string, Color> > points;
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

scene new_scene(std::vector<group> groups, std::vector<light> lights);

light new_empty_light();

light new_light(int type, int x, int y, int z);

group new_empty_group();

group new_group(std::vector<Transformation*> transformations, std::vector<std::string> points, std::vector<group> subgroups);

figure new_figure();

#endif
