#include "CollisionDetector.h"

bool CollisionDetector::operator()(const Player& player, const Enemy& e) {
	auto d = (player.getSize() + e.getSize()) / 2;
	if (abs(player.getCenter()[0] - e.getCenter()[0]) < d && abs(player.getCenter()[1] - e.getCenter()[1]) < d) {
		std::cout << "적 충돌 발생\n";
		return true;
	}

	else
		return false;
}