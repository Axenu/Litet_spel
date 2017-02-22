#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"



void Guard::setLevel(Grid *level)
{
	this->_currentLevel = level;
}

void Guard::update(float dt)
{
	/*
	glm::vec3 pos = _position;
	if (_path->walkOnPath(&pos, _speed, dt))
	{
		glm::ivec2 start = _currentLevel->getSquare(this->getWorldPos());
		_path = _currentLevel->generatePath(start, _currentLevel->getRandomSquare());
	}
	if (pos.x < 0 || pos.z < 0)
		int a = 0;
	setPosition(pos);
	face(_path->movingTo());
	*/
	face(_player->getWorldPos());
	GameObject::update(dt); //Let object update the move vars before doing our update logic

	if (glm::length(this->getWorldPos() - _player->getWorldPos()) < GUARDVIEWDISTANCE)
	{
		if (DetectedPlayer())
		{
			//std::cout << "Detected player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			GameOverEvent event(false);
			//_eventManager->execute(event);

			glm::vec3 detP = _player->getWorldPos();
			std::cout << "Detected :" << detP.x << ", " << detP.y << ", " << detP.z << '\n';
		}
	}

}

Guard::Guard(glm::vec3 position, Character* player, EventManager* event, Model &m, Grid *gridet) :
	GameObject(m), _player(player), _eventManager(event)
{
	setPosition(position);
	_detectFov = std::cos(GUARDFOV);

	glm::ivec2 start = gridet->getSquare(this->getWorldPos());
	_path = gridet->generatePath(start, gridet->getRandomSquare());

	_speed = 0.4f;
}

Guard::~Guard()
{
}

bool Guard::DetectedPlayer()
{

	glm::vec3 pos = this->getWorldPos();
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 playerEyePos = _player->getEyePos();

	pos.y = 1.3f;

	glm::vec3 guardToPlayer = playerPos - pos;
	float playerDist = glm::length(guardToPlayer);
	guardToPlayer = glm::normalize(guardToPlayer);


	//Player to close detect!
	if (playerDist < 1.2f)
		return true;
	glm::vec3 forward = getForward();

	if (glm::dot(guardToPlayer, forward) > _detectFov)
	{
		float playerLight = 1.0f;
		//Calculate light at position
		playerLight = glm::min(_currentLevel->calcLightOnPosition(playerPos), 1.0f);
		//Account light for Anti-L Grenade
		if (glm::length(playerPos - glm::vec3(_player->getGrenadeData()._grenadePositionWhenLanded)) < _player->getGrenadeData().expanding)
			playerLight *= _player->getGrenadeData().fading;
		//Distance to stuff
		float wallDist = _currentLevel->getWallDist(pos, forward, GUARDVIEWDISTANCE * playerLight);
		float objectDist = _currentLevel->getObjectDist(pos, forward, GUARDVIEWDISTANCE * playerLight, playerPos);
		//If player closer then stuff, detect!
		if (playerDist < wallDist && playerDist < objectDist)
			return true;
	}
	return false;
	/*
	glm::vec3 upVector(0.f, 1.f, 0.f);
	bool result = false;
	unsigned int hitCounter = 0;
	glm::vec3 rightVector = glm::normalize(glm::cross(upVector, guardToPlayer)) * 0.2f;

	glm::vec3 upperLeft = playerPos - rightVector;
	glm::vec3 upperRight = playerPos + rightVector;
	glm::vec3 lowerLeft = playerPos - upVector - rightVector;
	glm::vec3 lowerRight = playerPos - upVector + rightVector;

	glm::vec3 rayUpperLeft = glm::normalize(upperLeft - pos);
	glm::vec3 rayUpperRight = glm::normalize(upperRight - pos);
	glm::vec3 rayLowerLeft = glm::normalize(lowerLeft - pos);
	glm::vec3 rayLowerRight = glm::normalize(lowerRight - pos);
	if (acos(glm::dot(rayUpperLeft, forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayLowerRight, forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayUpperRight, forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayLowerLeft, forward)) < GUARDFOV)
	{
		hitCounter++;
	}

	if (hitCounter > 1)
	{
		playerLight = glm::min(_currentLevel->calcLightOnPosition(playerPos), 1.0f);
		
		if (glm::length(playerPos - glm::vec3(_player->getGrenadeData()._grenadePositionWhenLanded)) < _player->getGrenadeData().expanding)
		{
			playerLight *= _player->getGrenadeData().fading;
		}

		float wallDist = _currentLevel->getWallDist(pos, rayUpperLeft, GUARDVIEWDISTANCE * playerLight);
		float objectDist = _currentLevel->getObjectDist(pos, rayLowerLeft, GUARDVIEWDISTANCE * playerLight, playerPos);

		if (playerDist < wallDist || playerDist < objectDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && objectDist == 0.0f && playerDist < (GUARDVIEWDISTANCE * playerLight))
		{
			result = true;
		}

		wallDist =_currentLevel->getWallDist(pos, rayUpperRight, GUARDVIEWDISTANCE * playerLight);
		objectDist = _currentLevel->getObjectDist(pos, rayLowerLeft, GUARDVIEWDISTANCE * playerLight, playerPos);

		if (playerDist < wallDist || playerDist < objectDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && objectDist == 0.0f && playerDist < (GUARDVIEWDISTANCE * playerLight))
		{
			result = true;
		}
	}
	return result;
	*/
}