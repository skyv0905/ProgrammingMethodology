#include "CollisionHandler.h"
#include <chrono>

extern bool blinking;
extern clock_t blinkStartTime;

extern long long getCurrentTime();

void CollisionHandler::operator()(Player& player, vector<Enemy>& enemys) {

	CollisionDetector playerenemycollision;

	for (auto& e : enemys) {
		if (playerenemycollision(player, e)) {

			player.setUnderAttack(true);
		}
	}

	player.setUnderAttack(false);
}