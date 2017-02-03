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

int LootController::loot(Camera & cam, float pickDist)
{
	float testDist;
	std::vector<int> indices;
	std::vector<float> distance;
	//Sorting out objects close enough to cam
	for (unsigned int i = 0; i < _loot.size(); i++)
	{
		testDist = cam.getDistance(_loot[i]);
		if (testDist < pickDist)
		{
			indices.push_back(i);
			distance.push_back(testDist);
		}
	}
	//Sorting the lists with closest to camera first
	int swapIndice;
	for (unsigned int i = 0; i < distance.size(); i++)
	{
		testDist = distance[i];
		swapIndice = i;
		for (unsigned int j = (i + 1); j < distance.size(); j++)
		{
			if (distance[j] < testDist)
			{
				swapIndice = j;
			}
		}
		if (swapIndice != i)
		{
			std::iter_swap(indices.begin() + i, indices.begin() + swapIndice);
			std::iter_swap(distance.begin() + i, distance.begin() + swapIndice);
		}
	}
	//Pick against the lootobjects in range, from closest to furthest
	int value;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		value = _loot[i].pick(cam);
		if (value != 0)
		{
			break;
		}
	}

	return value;
}
