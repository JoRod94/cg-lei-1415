#include "stdafx.h"
#include "skybox.h"
#include <glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <iostream>


using namespace std;


void Skybox::init_textures()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	vertexBuffer = (float *)malloc(sizeof(float) * 6 * 6 * 3);
	textureBuffer = (float *)malloc(sizeof(float) * 6 * 6 * 2);

	generateSkyboxVBO();

	glGenBuffers(1, &vertex);
	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * 3, vertexBuffer, GL_STATIC_DRAW);
	
	glGenBuffers(1, &tex);
	glBindBuffer(GL_ARRAY_BUFFER, tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * 2, textureBuffer, GL_STATIC_DRAW);

	for (int i = 0; i < 6; i++){
		create_clamp_texture(files[i], i);
	}


}

void Skybox::create_clamp_texture(const char *filepath, int i){
	ilGenImages(1, &images[i]);
	ilBindImage(images[i]);
	ilLoadImage((ILstring)filepath);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	int tw = ilGetInteger(IL_IMAGE_WIDTH);
	int th = ilGetInteger(IL_IMAGE_HEIGHT);
	unsigned char *texData = ilGetData();
	glGenTextures(1, &textures[i]);
	glBindTexture(GL_TEXTURE_2D, textures[i]);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Skybox::generateSkyboxVBO(){
	int v_count = 0;
	int t_count = 0;

	// front

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;


	// left

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;


	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;


	// back

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	// right

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	// top

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;


	// bottom

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 0;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = 10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 1;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 1;

	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = -10.0f;
	vertexBuffer[v_count++] = 10.0f;
	textureBuffer[t_count++] = 0;
	textureBuffer[t_count++] = 0;
}
 
void Skybox::draw(){

	glPushMatrix();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, tex);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	for (int i = 0; i < 6; i++){
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawArrays(GL_TRIANGLES, i*6, 6);
	}

	glPopAttrib();
	glPopMatrix();
}
