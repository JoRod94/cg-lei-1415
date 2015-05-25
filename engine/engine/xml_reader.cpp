#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <sys/stat.h>
#include "transformation.h"
#include "color.h"
#include "translation.h"
#include "scale.h"
#include "rotation.h"
#include "tinyxml2.h"
#include "xml_reader.h"
#include "engine.h"
#include "canvas.h"

using namespace std;

map<string, figure> m_files;
int active_buffer = 0;
int currLight = 0;

static bool file_exists(const char* filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

void read_bin(string filename, int texId){
	figure f = new_figure();

	map<string, figure>::iterator file = m_files.find(filename);

	if (file != m_files.end()) {
		free(f);
		cout << "MODEL FILE ALREADY READ: " << filename << endl;
		return;
	}

	ifstream i(filename, ios::binary);
	if (!i) {
		cout << "INEXISTENT FILE" << endl;
		return;
	}

	//positions
	i.read((char *)&(f->n_coords), sizeof(unsigned int));
	f->vertex = (float *)malloc((f->n_coords)*sizeof(float));
	i.read((char *)f->vertex , f->n_coords*sizeof(float));

	//indices
	i.read((char *)&(f->n_ind), sizeof(unsigned int));
	f->indices = (unsigned int *)malloc(f->n_ind*sizeof(unsigned int));
	i.read((char *)&(f->indices[0]), f->n_ind*sizeof(unsigned int));

	//normals
	f->normal = (float *)malloc((f->n_coords)*sizeof(float));
	i.read((char *)f->normal , f->n_coords*sizeof(float));

	//texture coordinates
	i.read((char *)&(f->n_tex_coords), sizeof(unsigned int));
	f->tex_coord = (float *)malloc((f->n_tex_coords)*sizeof(float));
	i.read((char *)f->tex_coord, f->n_tex_coords*sizeof(float));

	f->vertex_buffer_nr = active_buffer++;
	f->normal_buffer_nr = active_buffer++;
	f->image_texture_ID = texId;

	m_files[filename] = f;
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

static Color* get_model_colors(tinyxml2::XMLElement* model) {
	const char* diffR = model->Attribute(_XML_DIFF_R);
	const char* diffG = model->Attribute(_XML_DIFF_G);
	const char* diffB = model->Attribute(_XML_DIFF_B);
	// check if any exists
	// if none exists, return null
	// if one exists, the others default to zero
	// and we create a new color with the material flag on
	if ( diffR && diffG && diffB )
		return new Color(stof(diffR), stof(diffG), stof(diffB), true);
	return nullptr;
}

static bool valid_texture(const char* filename) {
	bool b = file_exists(filename);
	if (!b)
		cout << "INEXISTENT TEXTURE FILE, IGNORING..." << endl;

	return b;
}

int get_model_texture(tinyxml2::XMLElement* model) {
	const char* texture = model->Attribute(_XML_TEXTURE);
	if (texture && valid_texture(texture)){
		return 0; // change to id
		//GEN IMAGE E O RESTO DA TRALHA
	}
	else
		return -1;
}

static vector<pair<string, Color*> > group_points(tinyxml2::XMLElement* group) {
    vector<pair<string, Color*> > points;
    tinyxml2::XMLElement* models = group->FirstChildElement(_XML_MODELS);

	if (models) {
		for (tinyxml2::XMLElement* model = models->FirstChildElement(_XML_MODEL);
			model != NULL; model = model->NextSiblingElement(_XML_MODEL)) {
			string filename = model->Attribute(_XML_FILE);
			Color* c = get_model_colors(model);
			int texID = get_model_texture(model);
			read_bin(filename,texID);
			points.push_back(make_pair(filename, c));
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

bool parseGroup(tinyxml2::XMLElement* g, group &ret) {
    if(! valid_group(g)) {
        cout << "Invalid group found. Ignoring..." << endl;
        return false;
    }
    vector<Transformation*> t = group_transformations(g);
	vector<Transformation*> c = group_colors(g);
    vector<pair<string, Color*> > pt = group_points(g);
    vector<group> sg;

	for (int i = 0; i < c.size(); i++)
		t.push_back(c[i]);

	tinyxml2::XMLElement* subgroup = g->FirstChildElement(_XML_GROUP);
	while(subgroup != NULL) {
			group maybe_sub = (group)malloc(sizeof(struct s_group));


			if (parseGroup(subgroup, maybe_sub)) {
				sg.push_back(maybe_sub);
			}
			subgroup = subgroup->NextSiblingElement(_XML_GROUP);
       }

    ret = new_group(t, pt, sg);
    return true;
}

bool valid_light(tinyxml2::XMLElement* l) {
	const char* val = l->Attribute(_XML_LIGHT_TYPE);
    return val != NULL && (strcmp(val, _XML_LIGHT_POINT) == 0 || strcmp(val, _XML_LIGHT_VECTOR) == 0);
}

bool parse_light(tinyxml2::XMLElement* ls, light &ret) {
    if(! valid_light(ls) ) {
        cout << "Invalid light found, ignoring..." << endl;
        return false;
    }

	const char* s_type = ls->Attribute(_XML_LIGHT_TYPE);
    float type = strcmp(s_type, _XML_LIGHT_POINT ) == 0 ? 1.0f : 0.0f;
	ret = new_light(
		GL_LIGHT0 + currLight++,
		ls->FloatAttribute(_XML_LIGHT_X),
		ls->FloatAttribute(_XML_LIGHT_Y),
		ls->FloatAttribute(_XML_LIGHT_Z),
		type);
    
    return true;
}

scene parse_scene(tinyxml2::XMLElement* scene) {
    group ret_g = new_empty_group();
    light ret_l = new_empty_light();
    vector<group> groups;
    vector<light> lights;

    for (tinyxml2::XMLElement* g = scene->FirstChildElement(_XML_GROUP);
				g != NULL; g = g->NextSiblingElement(_XML_GROUP))
		{
			if ( parseGroup(g, ret_g) )
				groups.push_back(ret_g);
		}

    for (tinyxml2::XMLElement* ls = scene->FirstChildElement(_XML_LIGHTS);
            ls != NULL; ls = ls->NextSiblingElement(_XML_LIGHTS))
        for(tinyxml2::XMLElement* l = ls->FirstChildElement(_XML_LIGHT);
                l != NULL; l = l->NextSiblingElement(_XML_LIGHT))
            if( parse_light(l, ret_l) )
                lights.push_back(ret_l);

     return new_scene(groups, lights);
}

pair<vector<scene>, map<string, figure> > read_xml(char* xmlName) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xmlName);

    vector<scene> scenes;

    tinyxml2::XMLElement* camera = doc.FirstChildElement(_XML_CAMERA);

	if (camera) {
		set_camera(camera->FloatAttribute(_XML_CAM_ALPHA),
			camera->FloatAttribute(_XML_CAM_BETA),
			camera->FloatAttribute(_XML_CAM_RADIUS));
	}

	for (tinyxml2::XMLElement* s = doc.FirstChildElement(_XML_SCENE);
		s != NULL; s = s->NextSiblingElement(_XML_SCENE))
		scenes.push_back(parse_scene(s));

    return make_pair(scenes, m_files);
}

pair<vector<scene>, map<string, figure> > reset_and_read_xml(char* xmlName) {
	m_files.clear();
	active_buffer = 0;
	currLight = 0;
	return read_xml(xmlName);
}
