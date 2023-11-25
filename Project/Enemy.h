#pragma once

#include "Player.h"

class Enemy {
public:

	enum FACE { LEFT, RIGHT };
	enum HORIZONTAL_STATE { STOPH, MOVE };
	enum VERTICAL_STATE { STOPV, FALL };

	Enemy(float x, float y, float z, float size, FACE f);

	void setHorizontalState(HORIZONTAL_STATE hState);
	HORIZONTAL_STATE getHorizontalState();
	void setVerticalState(VERTICAL_STATE hState);

	void setCenter(const Vector3f& c);
	Vector3f getCenter() const;

	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;

	void setFace(FACE f);
	FACE getFace();

	float getSize() const;

	void move();
	void mMoveTick(); // 플레이어 초기 설정 이동 시간 프레임당 1씩 감소
	bool moveFinished() const; // return moveTice == 0

	void moveTo(Vector3f dst, float tick); // Vector3f를 받아서 tick시간안에 그 위치로 플레이어를 이동하게 velocity 설정

	void draw() const;
	void drawTexture(int face) const;

private:

	FACE face;
	Vector3f center;
	Vector3f velocity;
	float size;

	VERTICAL_STATE verticalState;
	HORIZONTAL_STATE horizontalState;

	int moveTick;
};

