#include "Game/Objects/LootObject.h"

LootObject::LootObject()
{
	_value = 50;
}

LootObject::LootObject(Model & m)
	:GameObject(m)
{
	_value = 50;
}

LootObject::LootObject(Model & m, int val)
	: GameObject(m)
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
