#pragma once
#include "GL/freeglut.h"
#include "Constants.h"
#include "Vector3f.h"
#include <string>

class Platform
{
public:
	enum class PLATFORM {LEFT, RIGHT, BOTTOM, NONE, MIDDLE};

	Platform();
	Platform(float x, float y, PLATFORM type, GLuint ID);
	
	PLATFORM getPlatformType();
	float getWidth() const;
	Vector3f getCenter() const;
	void setPlatform(float x, float y, PLATFORM type, GLuint ID);
	void draw();

private:
	GLuint textureID; // 텍스쳐 ID
	float width;
	Vector3f center;
	PLATFORM Platformtype;
};

