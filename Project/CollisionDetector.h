#pragma once

#include <vector>
#include <memory>
#include <math.h>
#include "Player.h"
#include "Enemy.h"
#include "Bubble.h"

using namespace std;

class CollisionDetector
{
public:
	bool operator()(const Player& player, const Enemy& e);
};
