#include <vector>
#include "Life.h"
#include "Texture.h"
#include "Constants.h"
#include <GL/freeglut.h>

extern std::vector<Texture> textures;

void Life::draw(int count) const {

	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[10].getTextureID());

	glPushMatrix();

	glTranslatef(center[0] + PIXEL * count, center[1], 0.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-(size / 2.0f), -size / 2.0f);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-(size / 2.0f), size / 2.0f);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f((size / 2.0f), size / 2.0f);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f((size / 2.0f), -size / 2.0f);
	glEnd();

	glPopMatrix();
}

void Life::drawTexture() const {

	glPopMatrix();
}