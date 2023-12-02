#include "Enemy.h"
#include "Constants.h"
#include <GL/freeglut.h>
#include "Texture.h"
#include "Stage.h"
#include <vector>

extern std::vector<Texture> textures;
extern std::vector<Stage> stages;

extern StageState state;
extern LoadState load;

enum FACE { LEFT, RIGHT };
enum HORIZONTAL_STATE { STOPH, MOVE };
enum VERTICAL_STATE { STOPV, FALL };

Enemy::Enemy(float x, float y, float z, float size, FACE f)  {
	origin[0] = x; origin[1] = y; origin[2] = z;
	center[0] = x; center[1] = boundaryY + size; center[2] = z;
	this->size = size;
	face = f;
	angle = 0;
	verticalState = FALL;
	horizontalState = STOPH;
	verticalState = STOPV;
}

float Enemy::getSize() const {

	return size;
}
/*----------------- Center ------------------*/

void Enemy::setCenter(const Vector3f& c) {
	center = c;
}

Vector3f Enemy::getCenter() const {

	return center;
}

/*----------------- Move ------------------*/

void Enemy::mMoveTick() {
	moveTick--;

	if (moveTick < 0) {
		moveTick = 0;
	}
}

bool Enemy::moveFinished() const { // return moveTice == 0
	return moveTick == 0;
}

void Enemy::moveTo(float tick) { // Vector3f를 받아서 tick프레임 안에 그 위치로 플레이어를 이동하게 velocity 설정
	Vector3f new_velocity(velocity);
	moveTick = tick;
	for (auto i = 0; i < 3; i++) {
		new_velocity[i] = (origin[i] - center[i]) / moveTick;
	}
	setVelocity(new_velocity);
}

//속도에 따라 Player의 위치를 update하는 함수. 본문의 idle function에 삽입.

void Enemy::move() {
	if (!isTrapped()) {
		center = center + velocity;

		if (moveFinished()) {

			for (auto itr = stages[static_cast<int>(state)].getStagePlatform().begin(); itr != stages[static_cast<int>(state)].getStagePlatform().end(); ++itr) {

				if ((this->center[1] - itr->getCenter()[1]) == (this->size / 2.f + PIXEL / 2.f) // 바닥 감지해서 방향 전환
					&& abs(this->center[0] - itr->getCenter()[0]) < PIXEL / 2) {

					if (this->face == RIGHT) {

						std::vector<Platform>::iterator next_itr = itr + 1;

						if ((next_itr->getCenter()[0] - itr->getCenter()[0]) > PIXEL) {
							face = LEFT;
							setVelocity(Vector3f(-5, 0, 0));
							break;
						}
					}

					else if (this->face == LEFT) {

						std::vector<Platform>::iterator before_itr = itr - 1;

						if ((itr->getCenter()[0] - before_itr->getCenter()[0]) > PIXEL) {
							face = RIGHT;
							setVelocity(Vector3f(5, 0, 0));
							break;
						}
					}
				}

				if (itr->getPlatformType() == Platform::PLATFORM::GROUND) { // 벽을 만나면
					auto center_p = itr->getCenter();
					auto d = (size + itr->getWidth()) / 2.0f;

					if (abs(center[0] - center_p[0]) < d && abs(center[1] - center_p[1]) < d) {
						if (face == LEFT) {
							face = RIGHT;
							setVelocity(Vector3f(5, 0, 0));
							break;
						}
						else{
							face = LEFT;
							setVelocity(Vector3f(-5, 0, 0));
							break;
						}
					}
				}
			}
			toInside();
		}
	}
}

void Enemy::toInside() {
	if (center[0] < -boundaryX) {
		center[0] = center[0] + WINDOW_WIDTH;
	}

	if (center[0] > boundaryX) {
		center[0] = center[0] - WINDOW_WIDTH;
	}

	if (center[1] < -boundaryY) {
		center[1] = center[1] + WINDOW_HEIGHT;
	}

	if (center[1] > boundaryY) {
		center[1] = center[1] - WINDOW_HEIGHT;
	}
}
/*----------------- Velocity ------------------*/

void Enemy::setVelocity(const Vector3f& v) {

	velocity = v;
}

Vector3f Enemy::getVelocity() const {

	return velocity;
}

/*----------------- Direction ------------------*/

void Enemy::setFace(FACE f) {

	face = f;
}

Enemy::FACE Enemy::getFace() {

	return face;
}

void Enemy::setHorizontalState(HORIZONTAL_STATE hState) {

	horizontalState = hState;

	Vector3f new_velocity(velocity);
	switch (horizontalState) {
	case MOVE:
		new_velocity[0] = 5.0f * ((face == LEFT) ? -1 : 1); // 좌우 이동 속도
		break;
	case STOPH:
		new_velocity[0] = 0.0f;
		break;
	}
	setVelocity(new_velocity);
}

Enemy::HORIZONTAL_STATE Enemy::getHorizontalState() {

	return horizontalState;
}

void Enemy::setVerticalState(VERTICAL_STATE hState) {

	verticalState = hState;

	Vector3f new_velocity(velocity);
	switch (verticalState) {
	case FALL:
		new_velocity[1] = -3.0f; // 낙하 속도
		break;
	case STOPV:
		new_velocity[1] = 0.0f;
		break;
	}
	setVelocity(new_velocity);
}

void Enemy::setExState(EX_STATE exState) {
	this->exState = exState;
	switch (exState) {
	case TRAPPED:
		size *= 0.8;
		break;
	}
}

bool Enemy::isDead() const {
	return exState == DEAD;
}

bool Enemy::isTrapped() const {
	return exState == TRAPPED;
}
/*----------------- Draw ------------------*/

void Enemy::draw() {

	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[8].getTextureID());

	auto f = face == LEFT ? -1 : 1;
	drawTexture(f);

	if (moveFinished()) {
		glPushMatrix(); // 주어진 범위 이탈 시 자연스럽게 보이기 위한 더미 이미지
		glTranslatef(-WINDOW_WIDTH, 0.0f, 0.0f);
		drawTexture(f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(WINDOW_WIDTH, 0.0f, 0.0f);
		drawTexture(f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, -WINDOW_HEIGHT, 0.0f);
		drawTexture(f);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0f, WINDOW_HEIGHT, 0.0f);
		drawTexture(f);
		glPopMatrix();
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Enemy::drawTexture(int face) {

	glPushMatrix();

	glTranslatef(center[0], center[1], 0);

	if (isTrapped()) { // 갇힌 상태에 회전작업
		glRotatef(angle, 0, 0, 1);
		//glScalef(0.8f, 0.8f, 0.8f);
		angle = angle + 0.5f;
	}

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(- (size / 2) * face, - size / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(- (size / 2) * face, size / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f((size / 2) * face, size / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f((size / 2) * face, - size / 2);
	glEnd();

	glPopMatrix();
}