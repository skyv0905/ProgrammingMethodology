#include "CollisionDetector.h"

bool CollisionDetector::operator()(const Player& player, const Enemy& enemy1, const Enemy& enemy2, const Enemy& enemy3) {

	if ( (( sqrt(pow(player.getCenter()[0] - enemy1.getCenter()[0], 2)) <= (player.getSize() / 2 + enemy1.getSize() / 2)) && (player.getCenter()[1] == enemy1.getCenter()[1]))
		|| ((sqrt(pow(player.getCenter()[0] - enemy2.getCenter()[0], 2)) <= (player.getSize() / 2 + enemy2.getSize() / 2)) && (player.getCenter()[1] == enemy2.getCenter()[1]))
		|| ((sqrt(pow(player.getCenter()[0] - enemy3.getCenter()[0], 2)) <= (player.getSize() / 2 + enemy3.getSize() / 2)) && (player.getCenter()[1] == enemy3.getCenter()[1])) )

		return true;

	else
		return false;
}