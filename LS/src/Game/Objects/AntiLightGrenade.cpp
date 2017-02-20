#include "Game/Objects/AntiLightGrenade.h"

AntiLightGrenade::AntiLightGrenade(Model &m) :
	GameObject(m)
{
	_grenadePositionWhenLanded=glm::vec4(-5,100.0f,-5.0f,0.f);
}

AntiLightGrenade::~AntiLightGrenade()
{

}
glm::vec4 AntiLightGrenade::getgrenadePositionWhenlanded()
{
	return _grenadePositionWhenLanded;
}
void AntiLightGrenade::update(float dt)
{
	_fallspeed = -0.01f;
	_velocity = 1.5f;
	if (QBeenPressed == false && QbeenActivated == true)
	{
		_movement = glm::vec3(_movement.x, _movement.y + _fallspeed, _movement.z);

		if (this->getPosition().y > 0.15)
		{
			this->move(_movement*dt);
		}
		else
		{
			_grenadePositionWhenLanded = this->getWorldPos();
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

		_movement = (_direction*_velocity);
		//	std::cout << "You pressed Q    "<<CharacterPositions.x<<","<<CharacterPositions.y<<","<<CharacterPositions.z<<" "<< std::endl;
		QbeenActivated = true;
		QBeenPressed = false;
	}
}
