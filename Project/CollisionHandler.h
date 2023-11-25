#pragma once

#include "CollisionDetector.h"

class CollisionHandler
{
public:
	void operator()(Player& player, Enemy& enemy, Enemy& enemy1, Enemy& enemy2);
};