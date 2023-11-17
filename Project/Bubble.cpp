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

void Bubble::setState(STATE s) {
	bubbleState = s;
	if (bubbleState == UP) {
		size = 1.0f;
		velocity.setPos(0.0f, 3.0f, 0.0f);
	}
}

void Bubble::move() {
	if (bubbleState == GROWING) { // 커지는 중
		size += 1.0f / 10;
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

