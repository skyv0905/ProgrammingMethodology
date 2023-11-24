#include "Player.h"
#include "Constants.h"
#include <GL/freeglut.h>
#include "Texture.h"

#include <vector>

extern std::vector<Bubble>bubbles;
extern std::vector<Texture> textures;
extern bool bPressLeft;
extern bool bPressRight;

//Player 생성자. 기본값으로는 왼쪽을 보고 있으며, 멈추어있는 상태.
Player::Player(float x, float y, float z, float size) {

	center[0] = x; center[1] = y; center[2] = z;
	this->size = size;
	velocity[0] = 0; velocity[1] = 0, velocity[2] = 0;
	face = RIGHT;
	horizontalState = STOPH;
	verticalState = STOPV;
	bubbleCooldown = 0;
}

//Player의 중심 위치를 Vector3f 클래스의 객체를 받아 설정
void Player::setCenter(const Vector3f& c) {

	center = c;
}

//Player의 중심 위치를 반환하는 함수
Vector3f Player::getCenter() const {

	return center;
}

//Player의 속도를 Vector3f 클래스의 객체를 받아 설정하는 함수
void Player::setVelocity(const Vector3f& v) {

	velocity = v;
}

//Player의 속도를 반환하는 함수
Vector3f Player::getVelocity() const {

	return velocity;
}

//Player가 바라보는 방향을 enum FACE를 type으로 한 값으로 설정하는 함수
void Player::setFace(FACE f) {

	face = f;
}

//Player가 수평으로 움직일 때, 멈추어 있는지 움직이는지를 설정하는 함수
void Player::setHorizontalState(HORIZONTAL_STATE hState) {

	horizontalState = hState;
}

void Player::setVerticalState(VERTICAL_STATE hState) {

	verticalState = hState;
}

//Player가 쏜 버블을 Bubble을 class로 한 객체로 생성하는 함수. 생성 위치는 플레이어의 위치. 크기느 player와 같게. 속도의 부호는 player의 방향에 따라서. MTL은 일정하게.
Bubble Player::shootBubble() {

	Vector3f velocityright(25, 0, 0); // 버블 속도
	Vector3f velocityleft(-25, 0, 0);

	Bubble bub(PLAYER_SIZE/2, 16, 16);
	bub.setCenter(center);
	bub.setVelocity((face == LEFT) ? velocityleft : velocityright);

	Material m;

	m.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
	m.setAmbient(0.4f, 0.1f, 0.4f, 1.0f);
	m.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	m.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	m.setShininess(10.0f);

	bub.setMTL(m);
	
	bubbleCooldown = 0.5f; // 버블 재발동 대기시간

	return bub;
}

//Player가 움직이고 있으면 true, 아니면 false를 반환하는 함수
bool Player::isMoving() const {

	if (!velocity[0] && !velocity[1] && !velocity[2]) return false;
	else return true;
}

void Player::mBubbleCooldown() {
	bubbleCooldown -= 0.1f;
	
	if (bubbleCooldown < 0.0f) {
		bubbleCooldown = 0.0f;
	}
}

bool Player::canShootBubble() const { // 버블 발사 가능할 시 true 리턴
	return bubbleCooldown == 0.0f;
}

void Player::setAcceleration(Vector3f accel) {

	acceleration = accel;
}

bool Player::isJumping() const {

	return (verticalState == JUMP && velocity[1] < 0);
}

bool Player::isFalling() const {

	return (verticalState == FALL);
}

//속도에 따라 Player의 위치를 update하는 함수. 본문의 idle function에 삽입.

void Player::move() {

	if (horizontalState == MOVE) {

		center = center + velocity;
	}

	else if (horizontalState == STOPH) {

		Vector3f velocitystoph(0, velocity[1], velocity[2]);
		velocity = velocitystoph;

		center = center + velocity;
	}

	Vector3f zero(0, 0, 0);
	Vector3f acceleration(0, -1, 0);

	if (verticalState == JUMP) {

		setAcceleration(acceleration);

		if (isJumping()) {
			verticalState = FALL;
		}

		velocity = velocity + acceleration;
		center = center + velocity;
	}

	else if (verticalState == FALL) {

		setAcceleration(zero);

		Vector3f velocityfall(velocity[0], -3, velocity[2]);
		setVelocity(velocityfall);

		center = center + velocity;
	}

	else if (verticalState == STOPV) {

		setAcceleration(zero);

		Vector3f velocitystopv(velocity[0], 0, velocity[2]);
		velocity = velocitystopv;

		center = center + velocity;
	}
}

//Player를 그리는 함수. 왼쪽을 바라볼 때와 오른쪽을 바라볼 때 서로 다른 이미지로 mapping해야 함.

void Player::draw() const {

	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[7].getTextureID());

	glBegin(GL_QUADS);

	if (face == LEFT) {

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(center[0] - size / 2, center[1] - size / 2);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(center[0] - size / 2, center[1] + size / 2);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(center[0] + size / 2, center[1] + size / 2);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(center[0] + size / 2, center[1] - size / 2);
	}

	else {

		glTexCoord2f(1.0f, 0.0f);
			glVertex3f(center[0] - size / 2, center[1] - size / 2, center[2]);
		glTexCoord2f(1.0f, 1.0f);
			glVertex3f(center[0] - size / 2, center[1] + size / 2, center[2]);
		glTexCoord2f(0.0f, 1.0f);
			glVertex3f(center[0] + size / 2, center[1] + size / 2, center[2]);
		glTexCoord2f(0.0f, 0.0f);
			glVertex3f(center[0] + size / 2, center[1] - size / 2, center[2]);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}
