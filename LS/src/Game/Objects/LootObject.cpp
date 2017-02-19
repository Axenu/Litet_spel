#include "Game/Objects/LootObject.h"

LootObject::LootObject()
{
	_value = 50;
}

LootObject::LootObject(Model & m)
	:StaticGameObject(m)
{
	_value = 50;
}

LootObject::LootObject(Model & m, int val)
	:StaticGameObject(m)
{
	_value = val;
}

int LootObject::loot(Camera &cam)
{
	if (GameObject::pick(cam))
	{
		return _value;
	}
	else
	{
		return 0;  //0 missed picking
	}
}

LootObject::~LootObject()
{

}

void LootObject::setValue(int val)
{
	_value = val;
}

int LootObject::getValue()
{
	return _value;
}

void LootObject::update(float dt)
{
	StaticGameObject::update(dt);
}
