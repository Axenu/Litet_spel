#include "Game/Objects/AntiLightGrenade.h"

AntiLightGrenade::AntiLightGrenade(Model &m) :
	GameObject(m)
{
	_grenadeValue._grenadePositionWhenLanded = glm::vec4(-5, 100.0f, -5.0f, 0.f);
	_grenadeValue.expanding = 0;
	_grenadeValue.fading = 0.2f;
}

AntiLightGrenade::~AntiLightGrenade()
{

}
GrenadeValues AntiLightGrenade::getgrenadeData()
{
//	std::cout << "this is spam" << std::endl;
	if (_grenadeValue.expanding < 3 && TheBombHasBeenActivated==true)
	{
		_grenadeValue.expanding += 0.01f;
	}
	else if(_grenadeValue.fading < 1 && TheBombHasBeenActivated == true)
	{
		_grenadeValue.fading += 0.0002f;
	}
	return _grenadeValue;
}
void AntiLightGrenade::update(float dt)
{
	_fallspeed = -0.01f;
	_velocity = 1.5f;
	if (QBeenPressed == false && QbeenActivated == true)
	{
		_movement = glm::vec3(_movement.x, _movement.y + _fallspeed, _movement.z);

		if (this->getPosition().y > 0.15 && !_currentLevel->wallCollissionForGrenade(this->getWorldPos(), _movement * dt))
		{
			this->move(_movement*dt);
		}
		else
		{
			_grenadeValue._grenadePositionWhenLanded = this->getWorldPos();
			this->setPosition(-5, 1, -5);
			QBeenPressed = true;
			QbeenActivated = false;
			TheBombHasBeenActivated = true;
		}

	}
	GameObject::update(dt);
}

void AntiLightGrenade::setLevel(Grid *level)
{
	this->_currentLevel = level;
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
		TheBombHasBeenActivated = false;
		_grenadeValue.fading = 0.1f;
		_grenadeValue.expanding = 0;
	}
}
