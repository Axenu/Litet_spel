#include "Game/Objects/Guard.h"

Guard::~Guard()
{
	delete path;
}

Guard::Guard()
{

}

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

void Guard::WalkingBetweenFourPoints(float dt)
{
	glm::vec2 roundedPosition = glm::vec2(this->getPosition().x, this->getPosition().z);
	switch (_aiChoice)
	{
		case 1:
		{
			goToSquare(dt, _point1z);
			if (roundedPosition == glm::vec2(this->getPosition().x, this->getPosition().z))//roundedPosition == glm::vec2(_point1z.x, _point1z.z))
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
				_aiChoice = randomgenerator(4);
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
	if (path->walkOnPath(&_position, &_forward, &_currentRot, _speed, dt))
	{
		path->createPath(_currentLevel->getSquare(this->getWorldPos()), glm::ivec2(randomgenerator(_width) - 1, randomgenerator(_height) - 1));
	}

	setRX(_currentRot + 180.f * M_PI / 180.f); // silvertejp här
	
	glm::vec3 pos = this->getWorldPos();

	if (pos != glm::vec3(0.f, 0.f, 0.f) && this->DetectedPlayer())
	{
		std::cout << "Detected player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		//GameOverEvent event(false);
		//_eventManager->execute(event);
	}

	GameObject::update(dt);
}

Guard::Guard(Character* player, EventManager* event, Model &m, Grid *gridet) :
	GameObject(m), _player(player), _eventManager(event), _displacement(0.5f, 0.f, 0.5f)
{
	_forward = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 guardStartPosition=gridet->getLastValueOfGuardLocationsAndremovesit();
	srand((unsigned int)time(NULL));
	//x = h�jd z= bred
	gridet->Creategetheightandwidthpoint12(guardStartPosition);
	_point1z = gridet->getheightandwidthpoint12(0)+ _displacement;
	_point2z = gridet->getheightandwidthpoint12(1)+ _displacement;
	_point1x = gridet->getheightandwidthpoint12(2)+ _displacement;
	_point2x = gridet->getheightandwidthpoint12(3)+ _displacement;

	_displacement = glm::vec3(0.5f, 0.5f, 0.5f);

	_width = gridet->getWidth();
	_height = gridet->getHeight();

	path = new Path();
	path->setLevel(gridet);

	_guardsstartposition = guardStartPosition + _displacement;
	_guardsstartposition.y = 1.0f;
	
	this->setPosition(_guardsstartposition);	

	_speed = 0.4f;

	_aiChoice = randomgenerator(4);
	_currentRot = 0.f;
}

int Guard::randomgenerator(int randomNumber)
{
	return (int)rand() % randomNumber + 1;
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

	glm::vec3 guardToPlayer = playerPos - pos;

	float playerDist = glm::length(guardToPlayer);

	if (playerDist < 0.8f)
	{
		return true;
	}

	glm::vec3 upVector(0.f, 1.f, 0.f);
	glm::vec3 rightVector = glm::normalize(glm::cross(upVector, guardToPlayer)) * 0.2f;
	
	glm::vec3 upperLeft = playerPos - rightVector;
	glm::vec3 upperRight = playerPos + rightVector;
	glm::vec3 lowerLeft = playerPos - upVector - rightVector;
	glm::vec3 lowerRight = playerPos - upVector + rightVector;
	
	glm::vec3 rayUpperLeft = glm::normalize(upperLeft - pos);
	glm::vec3 rayUpperRight = glm::normalize(upperRight - pos);
	glm::vec3 rayLowerLeft = glm::normalize(lowerLeft - pos);
	glm::vec3 rayLowerRight = glm::normalize(lowerRight - pos);

	if (acos(glm::dot(rayUpperLeft, _forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayLowerRight, _forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayUpperRight, _forward)) < GUARDFOV)
	{
		hitCounter++;
	}
	if (acos(glm::dot(rayLowerLeft, _forward)) < GUARDFOV)
	{
		hitCounter++;
	}

	if (hitCounter > 1)
	{
		float wallDist = getWallDist(pos, rayUpperLeft);

		if (playerDist < wallDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && playerDist < GUARDVIEWDISTANCE)
		{
			result = true;
		}

		wallDist = getWallDist(pos, rayUpperRight);

		if (playerDist < wallDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && playerDist < GUARDVIEWDISTANCE)
		{
			result = true;
		}
	}

	return result;
}

float Guard::getWallDist(glm::vec3 pos, glm::vec3 ray)
{
	glm::vec2 rayPos(pos.x, pos.z);

	glm::vec3 point(0.0f);
	bool signX = signbit(ray.x);
	bool signZ = signbit(ray.z);

	float viewingRange = 0.f;
	float wallDist = 0.0f;

	while (viewingRange < 5.f * GRIDSPACE)
	{
		glm::ivec2 gridPos((int)(floor(rayPos.x)), (int)(floor(rayPos.y)));
		if (_currentLevel->gettype(gridPos.y, gridPos.x) == wall)
		{
			wallDist = glm::length(glm::vec2(pos.x, pos.z) - rayPos);
			break;
		}
		else
		{
			if (signX)
			{
				if (_currentLevel->gettype(gridPos.y, gridPos.x - 1) == wall)
				{
					glm::vec3 triangles[6];
					_currentLevel->getLeftQuad(triangles, gridPos.x, gridPos.y);
					if (TriangleIntersection(triangles[0], triangles[1], triangles[2], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
					if (TriangleIntersection(triangles[3], triangles[4], triangles[5], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
				}
			}
			else
			{
				if (_currentLevel->gettype(gridPos.y, gridPos.x + 1) == wall)
				{
					glm::vec3 triangles[6];
					_currentLevel->getRightQuad(triangles, gridPos.x, gridPos.y);
					if (TriangleIntersection(triangles[0], triangles[1], triangles[2], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
					if (TriangleIntersection(triangles[3], triangles[4], triangles[5], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
				}
			}
			if (signZ)
			{
				if (_currentLevel->gettype(gridPos.y - 1, gridPos.x) == wall)
				{
					glm::vec3 triangles[6];
					_currentLevel->getBackQuad(triangles, gridPos.x, gridPos.y);
					if (TriangleIntersection(triangles[0], triangles[1], triangles[2], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
					if (TriangleIntersection(triangles[3], triangles[4], triangles[5], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
				}
			}
			else
			{
				if (_currentLevel->gettype(gridPos.y + 1, gridPos.x) == wall)
				{
					glm::vec3 triangles[6];
					_currentLevel->getFrontQuad(triangles, gridPos.x, gridPos.y);
					if (TriangleIntersection(triangles[0], triangles[1], triangles[2], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
					if (TriangleIntersection(triangles[3], triangles[4], triangles[5], glm::vec3(rayPos.x, 0.0f, rayPos.y), ray, point))
					{
						wallDist = glm::length(glm::vec2(pos.x, pos.z) - glm::vec2(point.x, point.z));
						break;
					}
				}
			}
		}
		rayPos += glm::vec2(ray.x, ray.z) * GRIDSPACE;
		viewingRange += GRIDSPACE;
	}
	return wallDist;
}
