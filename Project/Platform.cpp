#include "Platform.h"
#include "Stage.h"

extern std::vector<Stage>stages;

Platform::Platform() {
	width = PIXEL;
	center[0] = center[1] = 0;
	Platformtype = PLATFORM::GROUND;
}

Platform::Platform(float x, float y, PLATFORM type, GLuint ID) {
	width = PIXEL;
	center[0] = (x - 14) * PIXEL + (width / 2);
	center[1] = (y - 14) * PIXEL + (width / 2);;
	this->Platformtype = type;
	this->textureID = ID;
}

void Platform::setPlatform(float x, float y, PLATFORM type, GLuint ID) {
	center[0] = x;
	center[1] = y;
	this->Platformtype = type;
	this->textureID = ID;
}

Platform::PLATFORM Platform::getPlatformType() {
	return Platformtype;
}


void Platform::draw() {
	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(center[0] - (width / 2), center[1] - (width / 2));
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(center[0] - (width / 2), center[1] + (width / 2));
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(center[0] + (width / 2), center[1] + (width / 2));
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(center[0] + (width / 2), center[1] - (width / 2));
	glEnd();
	glDisable(GL_TEXTURE_2D);
}