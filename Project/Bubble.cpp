#include "Bubble.h"
#include <GL/freeglut.h>

Bubble::Bubble() {
	bubbleState = GROWING;
	size = 1.0f / 10;
}

Bubble::Bubble(float r, int sl, int st) : radius(r), slice(sl), stack(st) {
	bubbleState = GROWING;
	size = 1.0f / 10;
}

void Bubble::setRadius(float r) {

	radius = r;
}

float Bubble::getRadius() const {

	return radius;
}

float Bubble::getSize() const {

	return size;
}

void Bubble::setSlice(int sl) {

	slice = sl;
}

void Bubble::setStack(int st) {

	stack = st;
}

void Bubble::setCenter(const Vector3f& c) {

	center = c;
}

Vector3f Bubble::getCenter() const {

	return center;
}

void Bubble::setVelocity(const Vector3f& v) {

	velocity = v;
}

Vector3f Bubble::getVelocity() const {

	return velocity;
}

void Bubble::setMTL(const Material& m) {

	mtl = m;
}

void Bubble::setState(STATE s) { // GROWING 속도는 Player.cpp에서 init
	bubbleState = s;
	if (bubbleState == UP) {
		size = 1.0f;
		velocity.setPos(0.0f, 30.0f, 0.0f); // 상승 속도
	}
	else if (bubbleState == STOP) {
		velocity.setPos(0.0f, 0.0f, 0.0f); // 정지 속도
	}
}

Bubble::STATE Bubble::getState() {
	return bubbleState;
}

void Bubble::handleCollision(Vector3f center, float x) {
	switch (bubbleState) {
	case GROWING: // 성장 상태에서 충돌 발생
		this->center[0] = center[0] + (this->center[0] > center[0] ? (radius + x) : -(radius + x));
		setState(UP);
		break;
	case UP: // 상승 상태에서 충돌 발생
		this->center[1] = center[1] - radius - x;
		setState(STOP);
		break;
	case STOP: // 정지 상태에서 충돌 발생
		break;
	}
}

void Bubble::move() {
	if (bubbleState == STOP) return; // 정지 상태

	if (bubbleState == GROWING) { // 커지는 중
		size += 1.0f / 10;
		if (size > 1.0f) {
			setState(UP);
		}
	}
	center = center + velocity;
}

void Bubble::draw() const {

	float emission[4] = { mtl.getEmission()[0], mtl.getEmission()[1], mtl.getEmission()[2], mtl.getEmission()[3] };
	float ambient[4] = { mtl.getAmbient()[0], mtl.getAmbient()[1], mtl.getAmbient()[2], mtl.getAmbient()[3] };
	float diffuse[4] = { mtl.getDiffuse()[0], mtl.getDiffuse()[1], mtl.getDiffuse()[2], mtl.getDiffuse()[3] };
	float specular[4] = { mtl.getSpecular()[0], mtl.getSpecular()[1], mtl.getSpecular()[2], mtl.getSpecular()[3] };
	float shininess[1] = { mtl.getShininess() };

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glScalef(size, size, size);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}

