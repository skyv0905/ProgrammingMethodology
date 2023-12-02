#pragma once

#include "CollisionDetector.h"

class CollisionHandler
{
public:
	void operator()(Player& player, vector<Enemy>& enemys);
};