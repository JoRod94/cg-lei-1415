#include "stdafx.h"
#include "skybox.h"
#include <GL/glut.h>
#include <IL\il.h>

void Skybox::init_textures()
{
	for (int i = 0; i < 6; i++){
		create_clamp_texture(files[i], i);
	}
}

void Skybox::create_clamp_texture(const char *filepath, int i){

	ilBindImage(images[i]);
	ilLoadImage((ILstring)filepath);
	unsigned int tw = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	unsigned char *texData = ilGetData();
	glGenTextures(1, &textures[i]);
	// unsigned int texID 
	glBindTexture(GL_TEXTURE_2D, textures[i]);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, texData);
}

void Skybox::draw(){

	glPushMatrix();
	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	// Just in case we set all vertices to white.
	glColor4f(1, 1, 1, 1);

	// front
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(20.0f, -20.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 0); glVertex3f(-20.0f, -20.0f, -20.0f);
	glEnd();

	// left
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(20.0f, -20.0f, 20.0f);
	glTexCoord2f(0, 1); glVertex3f(20.0f, 20.0f, 20.0f);
	glTexCoord2f(1, 1); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 0); glVertex3f(20.0f, -20.0f, -20.0f);
	glEnd();

	// back
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-20.0f, -20.0f, 20.0f);
	glTexCoord2f(0, 1); glVertex3f(-20.0f, 20.0f, 20.0f);
	glTexCoord2f(1, 1); glVertex3f(20.0f, 20.0f, 20.0f);
	glTexCoord2f(1, 0); glVertex3f(20.0f, -20.0f, 20.0f);

	glEnd();

	// right
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-20.0f, -20.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(-20.0f, 20.0f, 20.0f);
	glTexCoord2f(1, 0); glVertex3f(-20.0f, -20.0f, 20.0f);
	glEnd();

	// top
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-20.0f, 20.0f, 20.0f);
	glTexCoord2f(0, 1); glVertex3f(-20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 1); glVertex3f(20.0f, 20.0f, -20.0f);
	glTexCoord2f(1, 0); glVertex3f(20.0f, 20.0f, 20.0f);
	glEnd();

	// bottom
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-20.0f, -20.0f, -20.0f);
	glTexCoord2f(0, 1); glVertex3f(-20.0f, -20.0f, 20.0f);
	glTexCoord2f(1, 1); glVertex3f(20.0f, -20.0f, 20.0f);
	glTexCoord2f(1, 0); glVertex3f(20.0f, -20.0f, -20.0f);
	glEnd();
	
	glPopAttrib();
	glPopMatrix();
}
