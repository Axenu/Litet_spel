#pragma once

#include <glm/glm.hpp>
#include "GameObject.h"
#include "Game/camera.h"

class LootObject : public GameObject
{
public:
	LootObject();
	LootObject(Model &m);
	LootObject(Model &m, int val);
	int loot(Camera &cam);   //0 missed picking
	virtual ~LootObject();
	void setValue(int val);
	int getValue();
private:
	int _value;

};