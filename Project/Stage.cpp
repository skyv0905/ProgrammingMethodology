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
				Platform p(x, y, Platform::PLATFORM::GROUND, platform_texture_type1);
				platforms.push_back(p);
			}

			else if (info[i].substr(j, 2) == "▣") { // 중간 플랫폼
				Platform p(x, y, Platform::PLATFORM::MIDDLE, platform_texture_type2);
				platforms.push_back(p);
			}

			else if (info[i].substr(j, 2) == "▲") { // 플레이어 위치
				player_origin.setPos((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0);
			}

			else if (info[i].substr(j, 2) == "◀") {
				auto en = std::make_shared<Enemy>((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0, PLAYER_SIZE, Enemy::LEFT);
				enemys.push_back(en);
				enemy_count++;
			}

			else if (info[i].substr(j, 2) == "▶") {
				auto en = std::make_shared<Enemy>((x - 14) * PIXEL + (PLAYER_SIZE / 2.f), (y - 14) * PIXEL + (PLAYER_SIZE / 2.f), 0, PLAYER_SIZE, Enemy::RIGHT);
				enemys.push_back(en);
				enemy_count++;
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

std::vector<std::shared_ptr<Enemy>>& Stage::getStageEnemy() {
	return enemys;
}

bool Stage::stageEnds() {
	return enemys.size() == 0;
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

// DEAD된 ENEMY 제거
void Stage::deleteDeadEnemy() {
	for (int i = enemys.size(); i > 0; i--) {
		if (enemys[i - 1]->isDead()) {
			enemys.erase(enemys.begin() + i - 1);
		}
	}
}

Vector3f Stage::getPlayerOrigin() {
	return player_origin;
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