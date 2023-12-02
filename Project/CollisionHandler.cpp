#include "CollisionHandler.h"
#include <chrono>

void CollisionHandler::operator()(Player& player, vector<Enemy>& enemys) {

	CollisionDetector playerenemycollision;

	for (auto& e : enemys) {
		if (playerenemycollision(player, e)) {

			player.setUnderAttack(true);
		}
	}
}