#pragma once

#include "Vector3f.h"

class Enemy {
public:

	enum FACE { LEFT, RIGHT };
	enum HORIZONTAL_STATE { STOPH, MOVE };
	enum VERTICAL_STATE { STOPV, FALL };
	enum EX_STATE { ALIVE, TRAPPED, DEAD };

	Enemy(float x, float y, float z, float size, FACE f);

	void setHorizontalState(HORIZONTAL_STATE hState);
	HORIZONTAL_STATE getHorizontalState();
	void setVerticalState(VERTICAL_STATE hState);
	void setExState(EX_STATE exState);
	bool isDead() const;
	bool isTrapped() const;

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

	void moveTo(float tick); // tick시간안에 origin 위치로 플레이어를 이동하게 velocity 설정
	void toInside();
	void draw();
	void drawTexture(int face);

private:

	FACE face;
	Vector3f origin;
	Vector3f center;
	Vector3f velocity;
	float size;
	float angle;

	VERTICAL_STATE verticalState;
	HORIZONTAL_STATE horizontalState;
	EX_STATE exState;

	int moveTick;
};

