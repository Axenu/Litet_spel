#include "..\header\LootController.h"

LootController::LootController()
{
}

LootController::~LootController()
{
}

void LootController::addLoot(LootObject & loot)
{
	_loot.push_back(loot);
}

int LootController::loot(Camera & cam)
{
	for (unsigned int i = 0; i < _loot.size(); i++)
	{

	}
}
