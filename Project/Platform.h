#pragma once
#include "GL/freeglut.h"
#include "Constants.h"
#include "Vector3f.h"
#include <string>

class Platform
{
public:
	enum class PLATFORM {MIDDLE, GROUND};

	Platform();
	Platform(float x, float y, PLATFORM type, GLuint ID);
	
	PLATFORM getPlatformType();
	void setPlatform(float x, float y, PLATFORM type, GLuint ID);
	void draw();

private:
	GLuint textureID; // ÅØ½ºÃÄ ID
	float width;
	Vector3f center;
	PLATFORM Platformtype;
};

