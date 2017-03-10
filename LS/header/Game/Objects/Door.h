#pragma once

#include "GameObject.h"
#include "Game/Objects/GameObjectType.h"
#include "StaticGameObject.h"

class Door : public StaticGameObject
{
public:
	Door(Model & m, type::GameObjectType type);
	Door();
	~Door();
private:

};
