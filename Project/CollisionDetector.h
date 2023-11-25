#pragma once

#include <vector>
#include <math.h>
#include "Player.h"
#include "Enemy.h"
#include "Bubble.h"

using namespace std;

class CollisionDetector
{
public:
	bool operator()(const Player& player, const Enemy& enemy, const Enemy& enemy1, const Enemy& enemy2);
};
