#include "CollisionHandler.h"
#include <chrono>

extern bool blinking;
extern clock_t blinkStartTime;

extern long long getCurrentTime();

void CollisionHandler::operator()(Player& player, Enemy& enemy, Enemy& enemy1, Enemy& enemy2) {

	CollisionDetector playerenemycollision;

	if (playerenemycollision(player, enemy, enemy1, enemy2)) {

		player.setUnderAttack(true);
	}

	else {
		player.setUnderAttack(false);
	}
}