#include "Game/Objects/StaticGameObject.h"

StaticGameObject::StaticGameObject()
	:GameObject(type::GameObjectType::Static)
{

}

StaticGameObject::StaticGameObject(Model & m)
	:GameObject(m, type::GameObjectType::Static)
{

}

StaticGameObject::StaticGameObject(Model &m, type::GameObjectType type) : GameObject(m, type)
{

}

StaticGameObject::~StaticGameObject()
{

}

void StaticGameObject::update(float dT)
{

}
