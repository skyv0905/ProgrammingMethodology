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

			if (info[i].substr(j, 2) == "■") { // 단단 플랫폼

				if (j == 2) { Platform p(x, y, Platform::PLATFORM::LEFT, platform_texture_type1); platforms.push_back(p); }
				else if (j == info[i].size() - 4) { Platform p(x, y, Platform::PLATFORM::RIGHT, platform_texture_type1); platforms.push_back(p); }
				else if (i == 27) { Platform p(x, y, Platform::PLATFORM::BOTTOM, platform_texture_type1); platforms.push_back(p); }
				else { Platform p(x, y, Platform::PLATFORM::NONE, platform_texture_type1); platforms.push_back(p); }
			}

			else if (info[i].substr(j, 2) == "▣") { // 중간 플랫폼
				Platform p(x, y, Platform::PLATFORM::MIDDLE, platform_texture_type2);
				platforms.push_back(p);
			}

			else if (info[i].substr(j, 2) == "▲") { // 플레이어 위치
				player_origin.setPos((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0);
			}

			else if (info[i].substr(j, 2) == "◆") { 
				enemy_origin1.setPos((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0);
			}

			else if (info[i].substr(j, 2) == "★") {
				enemy_origin2.setPos((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0);
			}

			else if (info[i].substr(j, 2) == "◎") {
				enemy_origin3.setPos((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0);
			}

			x += 1.0f;
		}
		x = 0.0f;
		y += 1.0f;

	}
}
std::vector<Platform>& Stage::getStagePlatform() {

	return platforms;
}

float Stage::getFirstTransition() { // 화면 전환 관리
	return firstTransition;
}

float Stage::getSecondTransition() {
	return secondTransition;
}

void Stage::startFirstTransition() {
	firstTransition = -WINDOW_HEIGHT;
}

void Stage::startSecondTransition() {
	secondTransition = -WINDOW_HEIGHT;
}

Vector3f Stage::getPlayerOrigin() {
	return player_origin;
}

Vector3f Stage::getEnemyOrigin1() {
	return enemy_origin1;
}

Vector3f Stage::getEnemyOrigin2() {
	return enemy_origin2;
}

Vector3f Stage::getEnemyOrigin3() {
	return enemy_origin3;
}

void Stage::draw() {

	for (auto& p : platforms) {
		p.draw();
	}
}

void Stage::move() { // 화면 전환 관리

	if (firstTransition < 0) {
		firstTransition += (WINDOW_HEIGHT / 50); // 전환 속도
	}
	
	if (secondTransition < 0) {
		secondTransition += (WINDOW_HEIGHT / 50); // 전환 속도
	}

	if (firstTransition > 0 || secondTransition > 0) { // 에러 방지
		firstTransition = secondTransition = 0;
	}
}