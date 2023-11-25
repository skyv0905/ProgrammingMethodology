#include "Life.h"
#include <vector>
#include "Texture.h"
#include <GL/freeglut.h>

extern std::vector<Texture> textures;

void Life::draw() const {

	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[10].getTextureID());

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(center[0] - (size / 2), center[1] - size / 2);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(center[0] - (size / 2), center[1] + size / 2);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(center[0] + (size / 2), center[1] + size / 2);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(center[0] + (size / 2), center[1] - size / 2);
	glEnd();
}