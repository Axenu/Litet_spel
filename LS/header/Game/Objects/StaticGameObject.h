#pragma once

#include "Game/Objects/GameObject.h"

class StaticGameObject : public GameObject
{
public:
	StaticGameObject();
	StaticGameObject(Model &m);
	virtual ~StaticGameObject();
	virtual void update(float dT);
};