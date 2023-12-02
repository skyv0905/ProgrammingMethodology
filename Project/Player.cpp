#include "Player.h"
#include "Constants.h"
#include <GL/freeglut.h>
#include "Texture.h"

#include <vector>

extern std::vector<Bubble>bubbles;
extern std::vector<Texture> textures;
extern bool bPressLeft;
extern bool bPressRight;

extern void playMusicBubbleShotted();

//Player 생성자. 기본값으로는 왼쪽을 보고 있으며, 멈추어있는 상태.
Player::Player(float x, float y, float z, float size) {

	center[0] = x; center[1] = y; center[2] = z;
	this->size = size;
	velocity[0] = 0; velocity[1] = 0, velocity[2] = 0;
	center_before = center;
	face = RIGHT;
	horizontalState = STOPH;
	verticalState = STOPV;
	bubbleCooldown = 0;
	life = 3;
}

Player::EX_STATE Player::getExState() {

	return exState;
}

void Player::setUnderAttack(bool ua) {

	bUnderAttack = ua;
}

bool Player::getUnderAttack() {

	return bUnderAttack;
}

void Player::setLife(int life) {

	this->life = life;
}

int Player::getLife() {

	return life;
}

//Player의 중심 위치를 Vector3f 클래스의 객체를 받아 설정
void Player::setCenter(const Vector3f& c) {
	center = c;
}

//Player의 중심 위치를 반환하는 함수
Vector3f Player::getCenter() const {

	return center;
}

Vector3f Player::getCenterB() const {

	return center_before;
}

//Player의 속도를 Vector3f 클래스의 객체를 받아 설정하는 함수
void Player::setVelocity(const Vector3f& v) {

	velocity = v;
}

//Player의 속도를 반환하는 함수
Vector3f Player::getVelocity() const {

	return velocity;
}

void Player::setAcceleration(Vector3f accel) {

	acceleration = accel;
}

float Player::getSize() const{
	return size;
}

//Player가 바라보는 방향을 enum FACE를 type으로 한 값으로 설정하는 함수
void Player::setFace(FACE f) {

	face = f;
}

//Player가 수평으로 움직일 때, 멈추어 있는지 움직이는지를 설정하는 함수
void Player::setHorizontalState(HORIZONTAL_STATE hState) {

	horizontalState = hState;

	Vector3f new_velocity(velocity);
	switch (horizontalState) {
	case MOVE:
		new_velocity[0] = 7.0f * ((face == LEFT) ? -1 : 1); // 좌우 이동 속도
		break;
	case STOPH:
		new_velocity[0] = 0.0f;
		break;
	}
	setVelocity(new_velocity);
}

void Player::setVerticalState(VERTICAL_STATE hState) {

	verticalState = hState;

	Vector3f new_velocity(velocity);
	Vector3f new_acceleration;
	switch (verticalState) {
	case JUMP:
		new_velocity[1] = 15.0f; // 점프 속도
		new_acceleration.setPos(0.0f, -0.75f, 0.0f); // 점프 가속도
		break;
	case FALL:
		new_velocity[1] = -5.0f; // 낙하 속도
		new_acceleration.setPos(0.0f, 0.0f, 0.0f);
		break;
	case STOPV:
		new_velocity[1] = 0.0f;
		new_acceleration.setPos(0.0f, 0.0f, 0.0f);
		break;
	}
	setVelocity(new_velocity);
	setAcceleration(new_acceleration);
}

//Player가 좌우로 움직이고 있으면 true, 아니면 false를 반환하는 함수
bool Player::isMoving() const {

	return !(horizontalState == STOPH);
}

bool Player::isJumping() const {

	return (verticalState == JUMP);
}

bool Player::isFalling() const {

	return (verticalState == FALL);
}

void Player::setExState(EX_STATE eState) {

	exState = eState;
}

//Player가 쏜 버블을 Bubble을 class로 한 객체로 생성하는 함수. 생성 위치는 플레이어의 위치. 크기느 player와 같게. 속도의 부호는 player의 방향에 따라서. MTL은 일정하게.
Bubble Player::shootBubble() {

	Vector3f velocityright(25, 0, 0); // 버블 속도
	Vector3f velocityleft(-25, 0, 0);

	Bubble bub(PLAYER_SIZE/2, 16, 16);
	bub.setCenter(center);
	bub.setVelocity((face == LEFT) ? velocityleft : velocityright);

	Material m;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> r(0.0f, 1.0f);

	m.setEmission(0.1f, 0.1f, 0.1f, 1.0f);
	m.setAmbient(r(gen), r(gen), r(gen), 1.0f);
	m.setDiffuse(0.7f, 0.7f, 0.7f, 1.0f);
	m.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
	m.setShininess(10.0f);

	bub.setMTL(m);
	
	bubbleCooldown = 0.7f; // 버블 재발동 대기시간
	playMusicBubbleShotted();

	return bub;
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

void Player::mMoveTick() {
	moveTick--;

	if (moveTick < 0) {
		moveTick = 0;
	}
}

bool Player::moveFinished() const { // return moveTice == 0
	return moveTick == 0;
}

void Player::moveTo(Vector3f dst, float tick) { // Vector3f를 받아서 tick프레임 안에 그 위치로 플레이어를 이동하게 velocity 설정
	setVerticalState(FALL);
	Vector3f new_velocity;
	moveTick = tick;
	for (auto i = 0; i < 3; i++) {
		new_velocity[i] = (dst[i] - center[i]) / moveTick;
	}
	setVelocity(new_velocity);
}

//속도에 따라 Player의 위치를 update하는 함수. 본문의 idle function에 삽입.

void Player::move() {
	center_before = center;
	velocity = velocity + acceleration;
	center = center + velocity;
	toInside();
	return;
}

void Player::moveX() {
	center_before = center;
	velocity[0] = velocity[0] + acceleration[0];
	center[0] = center[0] + velocity[0];
	toInside();
	return;
}

void Player::moveY() {
	center_before = center;
	velocity[1] = velocity[1] + acceleration[1];
	center[1] = center[1] + velocity[1];
	toInside();
	return;
}

void Player::toInside() {
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

//Player를 그리는 함수. 왼쪽을 바라볼 때와 오른쪽을 바라볼 때 서로 다른 이미지로 mapping해야 함.

void Player::draw() const {
	glColor3f(1.0f, 1.0f, 1.0f); // 플레이어 히트박스 표시. 디버깅 용도
	glBegin(GL_LINE_LOOP);
	glVertex2f(center[0] - (size / 2), center[1] - size / 2);
	glVertex2f(center[0] - (size / 2), center[1] + size / 2);
	glVertex2f(center[0] + (size / 2), center[1] + size / 2);
	glVertex2f(center[0] + (size / 2), center[1] - size / 2);
	glEnd();

	glEnable(GL_TEXTURE_2D); // 텍스쳐작업
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[7].getTextureID());

	auto f = face == LEFT ? 1 : -1;
	drawTexture(f);

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

	glDisable(GL_TEXTURE_2D);
}

void Player::drawTexture(int face) const {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(center[0] - (size / 2) * face, center[1] - size / 2);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(center[0] - (size / 2) * face, center[1] + size / 2);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(center[0] + (size / 2) * face, center[1] + size / 2);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(center[0] + (size / 2) * face, center[1] - size / 2);
	glEnd();
}