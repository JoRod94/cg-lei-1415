#ifndef _XML_READER_H
#define _XML_READER_H

#include "canvas.h"
#include <utility>

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
#define _XML_TEXTURE		"textura"
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
#define _XML_LIGHTS			"luzes"
#define _XML_LIGHT			"luz"
#define _XML_LIGHT_X		"posX"
#define _XML_LIGHT_Y		"posY"
#define _XML_LIGHT_Z		"posZ"
#define _XML_LIGHT_VECTOR	"VECTOR"
#define _XML_LIGHT_POINT	"POINT"
#define _XML_COLOR_TYPE		"tipo"
#define _XML_SPECULAR		"SPEC"
#define _XML_AMB_DIFF		"AD"
#define _XML_EMISSION		"EM"
#define	_XML_DIFF_R			"diffR"
#define	_XML_DIFF_G			"diffG"
#define	_XML_DIFF_B			"diffB"
#define _XML_LIGHT_TYPE		"tipo"
#define _XML_CAM_RADIUS		"raio"
#define _XML_CAM_ALPHA		"alfa"
#define _XML_CAM_BETA		"beta"
#define _XML_SKYBOX			"skybox"
#define _XML_SKY_FRONT		"front"
#define _XML_SKY_LEFT		"left"
#define _XML_SKY_RIGHT		"right"
#define _XML_SKY_TOP		"top"
#define _XML_SKY_BOTTOM		"bottom"
#define _XML_SKY_BACK		"back"

using namespace std;

pair<vector<scene>, map<string, figure> > read_xml(char* xmlName);

pair<vector<scene>, map<string, figure> > reset_and_read_xml(char* xmlName);

#endif
