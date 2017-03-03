#include "Game/Objects/AntiLightGrenade.h"

AntiLightGrenade::AntiLightGrenade(Model &m) :
	GameObject(m)
{
	_grenadeValue._grenadePositionWhenLanded = glm::vec4(-100, 1.0f, -100.0f, 0.f);
	_grenadeValue.expanding = 0;
	_grenadeValue.fading = 0.2f;
	_timer = 0.0f;
	_qBeenPressed = true;
	_qbeenActivated = false;
	_theBombHasBeenActivated = false;
	_checkForSound = false;
}

AntiLightGrenade::~AntiLightGrenade()
{

}
GrenadeValues AntiLightGrenade::getgrenadeData()
{
//	std::cout << "this is spam" << std::endl;
	if (_grenadeValue.expanding < 3 && _theBombHasBeenActivated==true)
	{
		_grenadeValue.expanding += 0.01f;
	}
	else if(_grenadeValue.fading < 1 && _theBombHasBeenActivated == true)
	{
		_grenadeValue.fading += 0.0002f;
	}
	return _grenadeValue;
}
void AntiLightGrenade::update(float dt)
{
	_checkForSound = false;
	_fallspeed = -0.02f;
	_velocity = 5.f;
	if (_qBeenPressed == false && _qbeenActivated == true)
	{
		_movement = glm::vec3(_movement.x, _movement.y + _fallspeed, _movement.z);

		if (!_currentLevel->wallCollissionForGrenade(this->getPosition(), _movement * dt))
		{
			this->move(_movement*dt);
		}
		else
		{
			_grenadeValue._grenadePositionWhenLanded = this->getWorldPos();
			this->setPosition(-100, 1, -100);
	//		_qBeenPressed = true;
			_qbeenActivated = false;
			_theBombHasBeenActivated = true;
			_checkForSound = true;
			
			
		}

	}
	GameObject::update(dt);
}

void AntiLightGrenade::setLevel(Grid *level)
{
	this->_currentLevel = level;
}

bool AntiLightGrenade::getExplodedGrenade()
{
	return _checkForSound;
}

void AntiLightGrenade::ThrowTheLightgrenade(glm::vec3 CharacterPositions, glm::vec3 Direction)
{
	_checkForSound = false;
	if (_qBeenPressed == true)
	{
		_direction = Direction;
		this->setPosition(CharacterPositions.x, 1, CharacterPositions.z);

		_movement = (_direction*_velocity);
		//	std::cout << "You pressed Q    "<<CharacterPositions.x<<","<<CharacterPositions.y<<","<<CharacterPositions.z<<" "<< std::endl;
		_qbeenActivated = true;
		_qBeenPressed = false;
		_theBombHasBeenActivated = false;
		_grenadeValue.fading = 0.1f;
		_grenadeValue.expanding = 0;
	}
}
