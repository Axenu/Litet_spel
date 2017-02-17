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
	_fallspeed = -0.01f;
	_velocity = 1.5f;
	if (QBeenPressed == false && QbeenActivated == true)
	{
		movement = glm::vec3(movement.x, movement.y + _fallspeed, movement.z);

		if (this->getPosition().y > 0.15)
		{
			glm::vec3 swag = this->getPosition();
		//	std::cout << swag.x << "," << swag.y << "," << swag.z << std::endl;
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

	if (QBeenPressed == true)
	{
		_direction = Direction;
		this->setPosition(CharacterPositions.x, 1, CharacterPositions.z);

		movement = (_direction*_velocity);
		//	std::cout << "You pressed Q    "<<CharacterPositions.x<<","<<CharacterPositions.y<<","<<CharacterPositions.z<<" "<< std::endl;
		QbeenActivated = true;
		QBeenPressed = false;
	}
}