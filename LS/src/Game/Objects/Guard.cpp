#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"

/*
void Guard::print()
{
	for (int m = 0;m < _width;m++)
	{
		if (m < 10)
			std::cout << "W" << m << " ";
		else
			std::cout << m << " ";
	}
	std::cout << "" << std::endl;
	int row = 0;
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			if (_currentLevel->getvalue(j,i) == -1 || _currentLevel->getvalue(j, i) == -2)
			{
				std::cout << _currentLevel->getvalue(j, i) << " ";
			}
			else if (_currentLevel->getvalue(j, i) >=0 && _currentLevel->getvalue(j, i) < 10)
			{
				std::cout << "+" << _currentLevel->getvalue(j, i) << " ";
			}
			else
				std::cout << _currentLevel->getvalue(j, i) << " ";

			if (i == _width - 1)
			{
				std::cout << "R" << row;
				row = row + 1;
			}
		}
		std::cout << "" << std::endl;
	}
	std::cout << "end" << std::endl;
}
*/
void Guard::WalkingBetweenFourPoints(float dt)
{
	glm::vec2 roundedPosition = glm::vec2(this->getPosition().x, this->getPosition().z);
	switch (_aiChoice)
	{
		case 1:
		{
			goToSquare(dt, _point1z);
			if (roundedPosition == glm::vec2(this->getPosition().x, this->getPosition().z))
			{
				_aiChoice = -1;
			}
			break;
		}
		case -1:
		{
			goToSquare(dt, _guardsstartposition);
			if (roundedPosition == glm::vec2(_guardsstartposition.x, _guardsstartposition.z))
			{
				_aiChoice = getRand(5);
			}
			break;
		}
		case 2:
		{
			goToSquare(dt, _point2z);
			if (roundedPosition == glm::vec2(_point2z.x, _point2z.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		case 3:
		{
			goToSquare(dt, _point2x);
			if (roundedPosition == glm::vec2(_point2x.x, _point2x.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		case 4:
		{
			goToSquare(dt, _point1x);
			if (roundedPosition == glm::vec2(_point1x.x, _point1x.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		default:
		{
			std::cout << "something went horribley wrong here AI HAS SOME FLAWS MATE" << std::endl;
		}
	}
}

void Guard::setLevel(Grid *level)
{
	this->_currentLevel = level;
}

void Guard::update(float dt)
{
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
	if (glm::length(this->getWorldPos() - _player->getWorldPos()) < GUARDVIEWDISTANCE)
	{
		if (DetectedPlayer())
		{
			//std::cout << "Detected player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			GameOverEvent event(false);
			//_eventManager->execute(event);
			std::cout << "Detected :" << pos.x << ", " << pos.y << ", " << pos.z << '\n';
		}
	}

	GameObject::update(dt);
}

Guard::Guard(glm::vec3 position, Character* player, EventManager* event, Model &m, Grid *gridet) :
	GameObject(m), _player(player), _eventManager(event)
{
	setPosition(position);
	_guardsstartposition = position;
	 
	gridet->Creategetheightandwidthpoint12(position);
	_point1z = gridet->getheightandwidthpoint12(0) + glm::vec3(0.5f, 0.f, 0.5f);
	_point2z = gridet->getheightandwidthpoint12(1) + glm::vec3(0.5f, 0.f, 0.5f);
	_point1x = gridet->getheightandwidthpoint12(2) + glm::vec3(0.5f, 0.f, 0.5f);
	_point2x = gridet->getheightandwidthpoint12(3) + glm::vec3(0.5f, 0.f, 0.5f);

	glm::ivec2 start = gridet->getSquare(this->getWorldPos());
	_path = gridet->generatePath(start, gridet->getRandomSquare());

	_speed = 0.4f;

	_aiChoice = getRand(5);
}

Guard::~Guard()
{
	
}

void Guard::setPositionfromMap(glm::vec3 Guarden)
{
	this->setPosition(Guarden);
}

void Guard::goToSquare(float dt, glm::vec3 walkTo)
{
	glm::vec3 value = this->getPosition();
	glm::vec3 distance = walkTo - value;
	distance = glm::vec3(distance.x, 0, distance.z);
	_distLength = glm::length(distance);

	if (distance.z > 0)
	{
		if (_distLength < (_speed*dt))
		{
			this->setPosition(value.x,value.y,walkTo.z);
		
		}
		else
		{
			this->move(glm::vec3(0, 0, _speed) * dt);
		}
	}
	if (distance.z < 0)
	{
		if (_distLength < (_speed*dt))
		{
			this->setPosition(value.x, value.y, walkTo.z);
		
		}
		else
		{
			this->move(glm::vec3(0, 0, -_speed) * dt);
		}
	}
	if (distance.x > 0)
	{
		if (_distLength < (_speed*dt) )
		{
			this->setPosition(walkTo.x, value.y, value.z);
		
		}
		else
		{
		this->move(glm::vec3(_speed, 0,0) * dt);
		}
	}
	if (distance.x < 0)
	{
		if (_distLength < (_speed*dt))
		{
			this->setPosition(walkTo.x, value.y, value.z);
			
		}
		else
		{
		this->move(glm::vec3(-_speed, 0, 0) * dt);
		}
	}
}

bool Guard::DetectedPlayer()
{
	bool result = false;
	unsigned int hitCounter = 0;

	glm::vec3 pos = this->getWorldPos();
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 playerEyePos = _player->getEyePos();

	pos.y = 1.3f;

	glm::vec3 guardToPlayer = playerPos - pos;

	float playerLight = 1.0f;

	float playerDist = glm::length(guardToPlayer);

	if (playerDist < 1.5f)
	{
		return true;
	}

	glm::vec3 upVector(0.f, 1.f, 0.f);
	glm::vec3 forward = getForward();
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
}