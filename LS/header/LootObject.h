#pragma once

#include "GameObject.h"

class LootObject : public GameObject
{
public:
	LootObject();
	LootObject(Model &m);
	~LootObject();
	void setValue(int val);
private:
	int _value;

};