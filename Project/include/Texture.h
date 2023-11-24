#pragma once

#include <GL/freeglut.h>
#include <GL/FreeImage.h>

class Texture
{
public:
	//<functions>
	Texture() {}

	FIBITMAP* createBitMap(char const* filename);
	void generateTexture();
	void initializeTexture(char const* filename);

	GLuint getTextureID() const;

	//<variables>
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
};

