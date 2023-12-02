#include "Particle.h"
#include "GL/freeglut.h"
#include <random>

Particle::Particle() {
	Particle(Vector3f(0.0f, 0.0f, 0.0f), 0);
}

Particle::Particle(Vector3f cen, int flag) {
	center = cen;
	valid = 30;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> v(5.0f, 7.0f);
	std::uniform_real_distribution<float> r(-4.0f, -1.0f);
	std::uniform_real_distribution<float> c(0.2f, 1.0f);

	velocity.setPos(v(gen), 16.0f, 0.0f);
	accel.setPos(0.0f, r(gen), 0.0f);
	color.setPos(c(gen), c(gen), c(gen));

	switch (flag) {
	case 0:
		break;
	case 1:
		velocity[0] = velocity[0] * -1;
		break;
	}
}

void Particle::move() {
	velocity = velocity + accel;
	center = center + velocity;
	valid--;
}

void Particle::draw() const {
	if (valid <= 0) {
		return;
	}

	glPushMatrix();
	glColor3f(color[0], color[1], color[2]);

	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glVertex3f(center[0], center[1], center[2]);
	glEnd();
}