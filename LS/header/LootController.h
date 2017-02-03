#pragma once

#include <vector>
#include "LootObject.h"
#include "camera.h"


class LootController
{
public:
	LootController();
	~LootController();
	void addLoot(LootObject &loot);
	int loot(Camera &cam);
private:
	std::vector<LootObject> _loot;
};