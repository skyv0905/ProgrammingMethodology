#include "CollisionDetector.h"

bool CollisionDetector::operator()(const Player& player, const Enemy& e) {
	if (e.isTrapped() || e.isDead()) return false; // 적이 활동 중이 아닐 때는 false 리턴

	auto d = (player.getSize() + e.getSize()) / 2;
	if (abs(player.getCenter()[0] - e.getCenter()[0]) < d && abs(player.getCenter()[1] - e.getCenter()[1]) < d) {
		std::cout << "적 충돌 발생\n";
		return true;
	}

	else
		return false;
}