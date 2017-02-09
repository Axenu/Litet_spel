#include "Game/Objects/LootObject.h"

LootObject::LootObject()
{
	_value = 50;
	_totRotX = 0.0f;
	_rotX = (float)M_PI;
}

LootObject::LootObject(Model & m)
	:GameObject(m)
{
	_value = 50;
	_totRotX = 0.0f;
	_rotX = (float)M_PI;
}

LootObject::LootObject(Model & m, int val)
	: GameObject(m)
{
	_value = val;
	_totRotX = 0.0f;
	_rotX = (float)M_PI;
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
	_totRotX += _rotX * dt;
	
	if (_totRotX > 2 * M_PIf)
	{
		_totRotX -= (float)(2 * M_PI);
	}

	setRX(_totRotX);

	GameObject::update(dt);
}
