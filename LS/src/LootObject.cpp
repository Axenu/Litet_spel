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

LootObject::~LootObject()
{

}

void LootObject::setValue(int val)
{
	_value = val;
}
