#pragma once
class Skybox
{
public:
	const char *files[6];
	unsigned int  images[6];
	unsigned int  textures[6];

	Skybox(const char *front, const char *left, const  char *back, const char *right, const char *top, const  char *bottom){
		files[0] = front;
		files[1] = left;
		files[2] = back;
		files[3] = right;
		files[4] = top;
		files[5] = bottom;
	}

	virtual void init_textures();
	virtual void create_clamp_texture(const char *filepath, int i);
	virtual void draw();
};

