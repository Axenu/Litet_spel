#include "Game/Objects/AntiLightGrenade.h"

AntiLightGrenade::AntiLightGrenade(Model &m) :
	GameObject(m)
{
	_grenadeValue._grenadePositionWhenLanded = glm::vec4(-100, 1.0f, -100.0f, 0.f);
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
	checkForSound = false;
	_fallspeed = -0.02f;
	_velocity = 5.f;
	if (QBeenPressed == false && QbeenActivated == true)
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
	//		QBeenPressed = true;
			QbeenActivated = false;
			TheBombHasBeenActivated = true;
			checkForSound = true;
			
			
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
	return checkForSound;
}

void AntiLightGrenade::ThrowTheLightgrenade(glm::vec3 CharacterPositions, glm::vec3 Direction)
{
	checkForSound = false;
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
