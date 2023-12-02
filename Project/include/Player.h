#pragma once

#include "Bubble.h"
#include <iostream>
#include <random>

class Player
{
public:
	enum FACE { LEFT, RIGHT }; // 바라보는 방향 상태
	enum HORIZONTAL_STATE { STOPH, MOVE }; // 좌우 이동 상태
	enum VERTICAL_STATE { STOPV, JUMP, FALL }; // 상하 이동 상태
	enum EX_STATE { FREE, FORCEMOVING, COLLISION }; // 기타 상태 #FREE : 제한 없음, #FORCEMOVING : 스테이지 시작 시 강제 이동 상태, #INVINCIBLE : 무적 상태

	Player(float x, float y, float z, float size);

	void setLife(int life);
	int getLife();

	void setCenter(const Vector3f& c);
	Vector3f getCenter() const;
	Vector3f getCenterB() const;
	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;
	void setAcceleration(Vector3f accel);
	float getSize() const;

	void setFace(FACE f);
	void setHorizontalState(HORIZONTAL_STATE hState);
	void setVerticalState(VERTICAL_STATE hState);
	void setExState(EX_STATE eState);

	EX_STATE getExState();

	void setUnderAttack(bool ua);
	bool getUnderAttack();

	void printState() {
		std::cout << "Horizontal: ";
		switch (horizontalState) {
		case STOPH:
			std::cout << "STOP ";
			break;
		case MOVE:
			std::cout << "MOVE ";
			break;
		}
		std::cout << "Vertical: ";
		switch (verticalState) {
		case STOPV:
			std::cout << "STOP ";
			break;
		case JUMP:
			std::cout << "JUMP ";
			break;
		case FALL:
			std::cout << "FALL ";
			break;
		}
		std::cout << std::endl;
	}

	bool isMoving() const;
	bool isJumping() const; // Jump 후 속도가 0 이하로 감소 시 Fall로 state 전환 위한 함수
	bool isFalling() const;

	Bubble shootBubble();
	void mBubbleCooldown(); // 버블 재발사 대기시간 프레임당 0.1f씩 감소
	bool canShootBubble() const;

	void mMoveTick(); // 플레이어 초기 설정 이동 시간 프레임당 1씩 감소
	bool moveFinished() const; // return moveTice == 0

	void moveTo(Vector3f dst, float tick); // Vector3f를 받아서 tick시간안에 그 위치로 플레이어를 이동하게 velocity 설정
	void move();
	void moveX();
	void moveY();
	void toInside();
	void draw() const;
	void drawTexture(int face) const;

protected:
	Vector3f center_before;
	Vector3f center;
	Vector3f velocity;
	Vector3f acceleration;
	float size;
	FACE face;
	HORIZONTAL_STATE horizontalState;
	VERTICAL_STATE verticalState;
	EX_STATE exState;
	float bubbleCooldown; // 버블 재발사 대기시간
	int moveTick; // 플레이어 초기 설정 이동 시간

	bool bUnderAttack;
	int life;
};

