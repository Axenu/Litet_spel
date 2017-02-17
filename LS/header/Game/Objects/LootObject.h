#pragma once

#include <glm/glm.hpp>
#include "Game/Objects/StaticGameObject.h"
#include "Game/camera.h"

class LootObject : public StaticGameObject
{
public:
	LootObject();
	LootObject(Model &m);
	LootObject(Model &m, int val);
	int loot(Camera &cam);   //0 missed picking
	virtual ~LootObject();
	void setValue(int val);
	int getValue();
	virtual void update(float);
private:
	int _value;

};
