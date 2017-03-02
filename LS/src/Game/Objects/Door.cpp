#include "Game/Objects/Door.h"

void Door::update(float dt)
{
	GameObject::update(dt);
}

Door::Door(Model & m, type::GameObjectType type) :
	GameObject(m, type)
{

}

Door::Door()
{

}

Door::~Door()
{

}
