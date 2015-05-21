#include "stdafx.h"
#include "canvas.h"
#include <utility>

using namespace std;

scene new_scene(vector<group> groups, vector<light> lights) {
    scene s = (scene)calloc(1,sizeof(s_scene));
	s->groups = groups;
	s->lights = new vector<light>(lights);
	return s;
}

light new_empty_light() {
    return (light)malloc( sizeof(struct s_light));
}

light new_light(GLenum lId, float* pos) {
    light l = new_empty_light();
	l->pos = (float*)calloc(4,sizeof(float));

    l->lId = lId;
    memcpy(l->pos,pos,4*sizeof(float));
    return l;
}

group new_empty_group() {
    return (group)calloc(sizeof(s_group), 1);
}

group new_group(vector<Transformation*> transformations, vector<pair<string, Color*> > points, vector<group> subgroups) {
	group g = new_empty_group();

	g->transformations = transformations;

	g->points = new vector<pair<string, Color*> >(points);

    g->subgroups = subgroups;

	return g;
}

figure new_figure() {
    return (figure)malloc(sizeof(struct s_figure));
}
