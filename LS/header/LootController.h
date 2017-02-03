#pragma once

#include <vector>
#include <algorithm>
#include "LootObject.h"
#include "camera.h"


class LootController
{
public:
	LootController();
	~LootController();
	void addLoot(LootObject &loot);
	int loot(Camera &cam, float pickDist);
private:
	std::vector<LootObject> _loot;
};