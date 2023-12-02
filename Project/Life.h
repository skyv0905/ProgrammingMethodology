#pragma once

#include "Vector3f.h"

class Life {

public:
	Life(int s, Vector3f c) : size(s), center(c) {}
	void draw(int count) const;
	void drawTexture() const;

private:

	int life;
	int size;
	Vector3f center;
};