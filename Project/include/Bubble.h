#pragma once

#include "Material.h"

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
	
	void setMTL(const Material& m);
	void setState(STATE s);
	STATE getState();

	void handleCollision(Vector3f center, float x);
	void move();
	void draw() const;

private:
	float radius;
	float size;
	int slice;
	int stack;

	Vector3f center;
	Vector3f velocity;
	Material mtl;

	STATE bubbleState;
};
