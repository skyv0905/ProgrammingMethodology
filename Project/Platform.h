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
	Platform(float x, float y, PLATFORM type);
	
	PLATFORM getPlatformType();
	void setPlatform(float x, float y, PLATFORM type);
	void draw();

private:
	//Texture texture;
	float width;
	float x;
	float y;
	PLATFORM Platformtype;
};

