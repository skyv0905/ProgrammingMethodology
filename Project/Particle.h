#pragma once

#include "Vector3f.h"

class Particle
{
public:
	Particle();
	Particle(Vector3f cen, int flag);

	void move();
	void draw() const;

	int valid;

private:
	Vector3f center;
	Vector3f velocity;
	Vector3f accel;
	Vector3f color;
};

