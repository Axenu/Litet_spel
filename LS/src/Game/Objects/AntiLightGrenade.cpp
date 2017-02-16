#include "Game/Objects/AntiLightGrenade.h"

AntiLightGrenade::AntiLightGrenade(Model &m) : 
	GameObject(m)
{

}

AntiLightGrenade::~AntiLightGrenade()
{

}
void AntiLightGrenade::update(float dt)
{
	if (QBeenPressed == false && QbeenActivated==true)
	{
		movement = (_direction*_velocity);
		movement = glm::vec3(movement.x, movement.y + _fallspeed, movement.z);
		if(this->getPosition().y>0)
		{
		this->move(movement*dt);
		}
		else
		{
			QBeenPressed = true;
			QbeenActivated = false;
		}
	}
	GameObject::update(dt);
}
void AntiLightGrenade::ThrowTheLightgrenade(glm::vec3 CharacterPositions, glm::vec3 Direction)
{
	if(QBeenPressed == true)
	{
		this->setPosition(CharacterPositions.x,0.5,CharacterPositions.z);
		_direction = Direction;
	std::cout << "You pressed Q    "<<CharacterPositions.x<<","<<CharacterPositions.y<<","<<CharacterPositions.z<<" "<< std::endl;
		QbeenActivated = true;
	QBeenPressed = false;
	}
}