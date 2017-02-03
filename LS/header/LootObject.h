#pragma once

#include <glm/glm.hpp>
#include "GameObject.h"
#include "camera.h"

class LootObject : public GameObject
{
public:
	LootObject();
	LootObject(Model &m);
	LootObject(Model &m, int val);
	int loot(Camera &cam);   //0 missed picking
	~LootObject();
	void setValue(int val);
private:
	int _value;

};