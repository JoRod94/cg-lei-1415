#include "canvas.h"

scene new_scene() {
    return (scene)malloc(sizeof(struct s_scene));
}

light new_empty_light() {
    return (light)malloc(sizeof(struct s_light));
}

light new_light(int type, int x, int y, int z) {
    light l = new_empty_light();

    l->type = type;
    l->x = x;
    l->y = y;
    l->z = z;

    return l;
}

group new_empty_group() {
    return (group)malloc(sizeof(struct s_group));
}

group new_group(vector<Transformation*> transformations, vector<string> points, vector<group> subgroups) {
	group g = (group)calloc(1, sizeof(struct s_group));

	g->transformations = transformations;

	g->points = points;

    g->subgroups = subgroups;

	return g;
}

figure new_figure() {
    return (figure)malloc(sizeof(struct s_figure));
}


