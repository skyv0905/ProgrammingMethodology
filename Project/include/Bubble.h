#pragma once

#include "Material.h"
#include "Constants.h"

class Bubble {
public:
	enum STATE { GROWING, UP, STOP };

	Bubble();
	Bubble(float r, int sl, int st);

	void setRadius(float r);
	float getRadius() const;
	float getSize() const;
	void setSlice(int sl);
	void setStack(int st);

	void setCenter(const Vector3f& c);
	Vector3f getCenter() const;
	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;
	bool isWillDeleted() const;
	void setDeleted();
	
	void setMTL(const Material& m);
	void setState(STATE s);
	STATE getState();

	void handleCollision(Vector3f center, float x);
	void move();
	void toInside();
	void draw() const;

private:
	float radius;
	float size;
	int slice;
	int stack;
	bool willDeleted; // 버블-플레이어 충돌 시 상태 저장용

	Vector3f center;
	Vector3f velocity;
	Material mtl;

	STATE bubbleState;
};
