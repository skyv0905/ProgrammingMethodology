#include <iostream>
#include "Stage.h"

void Stage::setStagePlatformTextureID(GLuint a, GLuint b, GLuint c) {
	platform_texture_type1 = a;
	platform_texture_type2 = b;
	platform_texture_type3 = c;
}

void Stage::setStagePlatform(std::vector<std::string> info) {
	float x = 0.0f, y = 0.0f;
	for (int i = 27; i >= 0; i--) {
		for (auto j = 0; j < info[i].size(); j++) {
			if ((j % 2) != 0) continue;

			if (info[i].substr(j, 2) == "■") {
				Platform p(x, y, Platform::PLATFORM::GROUND, platform_texture_type1);
				platforms.push_back(p);
			}
			else if (info[i].substr(j, 2) == "▣") {
				Platform p(x, y, Platform::PLATFORM::MIDDLE, platform_texture_type2);
				platforms.push_back(p);
			}
			x += 1.0f;
		}
		x = 0.0f;
		y += 1.0f;
	}
}
std::vector<Platform> Stage::getStagePlatform() {

	return platforms;
}

float Stage::getFirstTransition() { // 화면 전환 관리
	return firstTransition;
}

float Stage::getSecondTransition() {
	return secondTransition;
}

void Stage::startFirstTransition() {
	firstTransition = -700.0f;
}

void Stage::startSecondTransition() {
	secondTransition = 700.0f;
}

void Stage::draw() {

	for (auto& p : platforms) {
		p.draw();
	}
}

void Stage::move() { // 화면 전환 관리
	if (firstTransition) {
		firstTransition += +10.0f; // 전환 속도
	}
	
	if (secondTransition) {
		secondTransition -= 10.0f; // 전환 속도
	}
}