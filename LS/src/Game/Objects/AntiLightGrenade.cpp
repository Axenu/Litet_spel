#include "Game/Objects/AntiLightGrenade.h"
#include "Event/SceneEvents/SceneEvent.h"

AntiLightGrenade::AntiLightGrenade(EventManager &eventManager, Model &m, glm::vec3 CharacterPositions, glm::vec3 Direction) :
	GameObject(m), _eventManager(eventManager)
{
	ThrowTheLightgrenade(CharacterPositions, Direction);
}

AntiLightGrenade::~AntiLightGrenade()
{

}
GrenadeValues AntiLightGrenade::getgrenadeData()
{
	return _grenadeValue;
}
void AntiLightGrenade::update(float dt)
{
	_movement = glm::vec3(_movement.x, _movement.y + _fallspeed, _movement.z);

	if (_landed)
	{
		//Explode
		if (_grenadeValue.expanding < expandSize)
			_grenadeValue.expanding += dt * expandSize / expandDur;
		else if (_grenadeValue.fading < 1)
			_grenadeValue.fading += dt / fadeDur;
		else
		{
			RemoveSceneObject e(this);
			_eventManager.execute(e);
		}

		Node::update(dt);
		GameObject::setModelAABB(AABB(getWorldPos(), _grenadeValue.expanding));
	}
	else 
	{
		//Move
		if (!_currentLevel->wallCollissionForGrenade(this->getPosition(), _movement * dt))
			this->move(_movement*dt);
		else
			_landed = true;
		GameObject::update(dt);
	}
}
/* Add the object to the frame
*/
void AntiLightGrenade::addToFrame(DrawFrame &dF)
{
	if (_landed)
	{
		AntiLightValues val(glm::vec2(_grenadeValue.expanding, _grenadeValue.fading), this->getWorldPos());
		dF.add(val);
	}
	else
		GameObject::addToFrame(dF);
}

void AntiLightGrenade::setLevel(Grid *level)
{
	this->_currentLevel = level;
}

void AntiLightGrenade::ThrowTheLightgrenade(glm::vec3 CharacterPositions, glm::vec3 Direction)
{
	_landed = false;
	_fallspeed = -0.02f;
	_velocity = 5.f;
	_grenadeValue.fading = 0.1f;
	_grenadeValue.expanding = 0;
	_direction = Direction;
	this->setPosition(CharacterPositions.x, 1, CharacterPositions.z);

	_movement = (_direction * _velocity);
}
