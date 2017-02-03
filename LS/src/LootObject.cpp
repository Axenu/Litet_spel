#include "..\header\LootObject.h"

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
		return 0;  //-1 missed picking
	}
}

LootObject::~LootObject()
{

}

void LootObject::setValue(int val)
{
	_value = val;
}
