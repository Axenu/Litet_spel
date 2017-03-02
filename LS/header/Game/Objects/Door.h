#pragma once

#include "GameObject.h"
#include "Game/Objects/GameObjectType.h"
class Door : public GameObject
{
public:
	virtual void update(float dt);
	Door(Model & m, type::GameObjectType type);
	Door();
	~Door();
private:

};