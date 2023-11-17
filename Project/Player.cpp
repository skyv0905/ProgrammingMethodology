#include "Player.h"
#include "Constants.h"
#include <GL/freeglut.h>

#include <vector>

extern std::vector<Bubble>bubbles;

//Player 생성자. 기본값으로는 왼쪽을 보고 있으며, 멈추어있는 상태.
Player::Player(float x, float y, float z, float size) {

	center[0] = x; center[1] = y; center[2] = z;
	this->size = size;
	velocity[0] = 0; velocity[1] = 0, velocity[2] = 0;
	face = RIGHT;
	horizontalState = STOPH;
	verticalState = STOPV;
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

	Vector3f velocityright(10, 0, 0);
	Vector3f velocityleft(-10, 0, 0);

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

	return bub;
}

//Player가 움직이고 있으면 true, 아니면 false를 반환하는 함수
bool Player::isMoving() const {

	if (!velocity[0] && !velocity[1] && !velocity[2]) return false;
	else return true;
}

void Player::setAcceleration(Vector3f accel) {

	acceleration = accel;
}

//속도에 따라 Player의 위치를 update하는 함수. 본문의 idle function에 삽입.
void Player::move() {

	Vector3f v(0, -2, 0);
	setAcceleration(v);

	if (horizontalState == HORIZONTAL_STATE::MOVE || verticalState == VERTICAL_STATE::FALL) {

		center = center + velocity;
	}

	if (verticalState == VERTICAL_STATE::JUMP) {

		velocity = velocity + acceleration;
		center = center + velocity;
	}

}

//Player를 그리는 함수. 왼쪽을 바라볼 때와 오른쪽을 바라볼 때 서로 다른 이미지로 mapping해야 함.
void Player::draw() const {

	glColor3f(1.0f, 1.0f, 1.0f);
	glPointSize(10.0f);

	glBegin(GL_POINTS);
		glVertex3f((face == LEFT) ? center[0] - size / 2 : center[0] + size / 2, center[1], center[2]);
	glEnd();

	glColor3f(0.45f, 0.26f, 0.33f);

	glBegin(GL_QUADS);
		glVertex3f(center[0] - size / 2, center[1] + size / 2, center[2]);
		glVertex3f(center[0] + size / 2, center[1] + size / 2, center[2]);
		glVertex3f(center[0] + size / 2, center[1] - size / 2, center[2]);
		glVertex3f(center[0] - size / 2, center[1] - size / 2, center[2]);
	glEnd();

}
