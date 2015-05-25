#include "stdafx.h"
#include "canvas.h"
#include <utility>

using namespace std;

scene new_scene(vector<group> groups, vector<light> lights) {
    scene s = (scene)calloc(1,sizeof(s_scene));
	/*
	copy(groups.begin(), groups.end(), s->groups);
	copy(lights.begin(), lights.end(), s->lights);
	*/
	s->lights = lights;
	s->groups = groups;
	return s;
}

light new_empty_light() {
    return (light)calloc( 1, sizeof(struct s_light) );
}

light new_light(GLenum lId, float x, float y, float z, float type) {
    light l = new_empty_light();
	l->pos = (float*)calloc(4,sizeof(float));
	l->pos[0] = x;
	l->pos[1] = y;
	l->pos[2] = z;
	l->pos[3] = type;
	l->lId = lId;
    return l;
}

group new_empty_group() {
    return (group)calloc(sizeof(s_group), 1);
}

group new_group(vector<Transformation*> transformations, vector<pair<string, Color *> > points, vector<group> subgroups) {
	group g = new_empty_group();

	g->transformations = transformations;

	g->points = points;
	
	g->subgroups = subgroups;

	return g;
}

figure new_figure() {
    return (figure)malloc(sizeof(struct s_figure));
}
