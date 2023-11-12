#include "Platform.h"

Platform::Platform() {
	width = PIXEL;
	x = y = 0;
	type = PLATFORM::GROUND;
}

Platform::Platform(float x, float y, PLATFORM type) {
	width = PIXEL;
	this->x = x;
	this->y = y;
	this->type = type;
}

void Platform::setPlatform(float x, float y, PLATFORM type) {
	this->x = x;
	this->y = y;
	this->type = type;
}

Platform::PLATFORM Platform::getPlatformType() {
	return type;
}

void Platform::draw() {
	(type == PLATFORM::GROUND) ?
		glColor3f(1.0f, 0.0f, 1.0f) :
		glColor3f(0.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f((x - 14) * PIXEL, (y - 14) * PIXEL);
	glVertex2f((x - 14) * PIXEL + width, (y - 14) * PIXEL);
	glVertex2f((x - 14) * PIXEL + width, (y - 14) * PIXEL + width);
	glVertex2f((x - 14) * PIXEL, (y - 14) * PIXEL + width);
	glEnd();
}