#pragma once

#include "Bubble.h"

class Player
{
public:
	enum FACE { LEFT, RIGHT };
	enum  HORIZONTAL_STATE { STOPH , MOVE };
	enum  VERTICAL_STATE { STOPV, JUMP, FALL };

	Player(float x, float y, float z, float size);

	void setCenter(const Vector3f& c);
	Vector3f getCenter() const;
	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;

	void setFace(FACE f);
	void setHorizontalState(HORIZONTAL_STATE hState);
	void setVerticalState(VERTICAL_STATE hState);
	void setAcceleration(Vector3f accel);
	Bubble shootBubble();

	bool isMoving() const;

	void mBubbleCooldown(); // 버블 재발사 대기시간 프레임당 0.1f씩 감소
	float getBubbleCooldown() const;

	void move();
	void draw() const;

private:
	Vector3f center;
	Vector3f velocity;
	Vector3f acceleration;
	float size;
	FACE face;
	HORIZONTAL_STATE horizontalState;
	VERTICAL_STATE verticalState;
	float bubbleCooldown;
};

