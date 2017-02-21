#include "Game/Objects/Guard.h"

Guard::~Guard()
{

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
	std::cout <<""<<std::endl;
	int row = 0;
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{


			if (_currentLevel->getvalue(j,i) == -1 || _currentLevel->getvalue(j, i) == -2)
			{
				std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_currentLevel->getvalue(j, i) << " ";
			}
			else if (_currentLevel->getvalue(j, i) >=0 && _currentLevel->getvalue(j, i) < 10)
			{
				std::cout << "+" << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_currentLevel->getvalue(j, i) << " ";
			}
			else
				std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_currentLevel->getvalue(j, i) << " ";

			if (i == _width - 1)
			{
				std::cout <<"R"<< row;
				row = row + 1;
			}
		}
		std::cout << "" << std::endl;
	}
	std::cout << "end" << std::endl;
}

void Guard::StartGridBuild()
{
	_currentPath = generatingPath(glm::ivec2(randomgenerator(_widthLength) - 1, randomgenerator(_heightLength) - 1)); //ändra till random efteråt
//	_currentPath = generatingPath(glm::ivec2(randomgenerator(1) , randomgenerator(1))); //ändra till random efteråt
}

std::vector<glm::vec2> Guard::generatingPath(glm::ivec2 GoalPosition)
{
	_height = _heightLength;
	_width = _widthLength;
	_guardsstartposition = this->getPosition();
	glm::vec3 guardInGrid = _guardsstartposition - _displacement;
	int maxValue = _height *_width - 1;
	int oldMaxValue = 0;
	int value = 0;
	glm::ivec2 goalPos = glm::ivec2((int)GoalPosition.x, (int)GoalPosition.y);
	//-1 outforskadmark ej gåbart
	// -2 vägg
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			_currentLevel->setvalue(j, i, -1);
			//std::cout << levalues[i][j].xz.x << levalues[i][j].xz.y << std::endl;
		}
	}
	_currentLevel->setvalue((int)guardInGrid.z, (int)guardInGrid.x, 0);

	while (maxValue != 0)
	{
		oldMaxValue = maxValue;
		for (int j = 0; j < _height; j++)
		{
			for (int i = 0; i < _width; i++)
			{
				if (_currentLevel->gettype(j,i) == nothing || _currentLevel->gettype(j, i) == guard)
				{
					if (_currentLevel->getvalue(j, i) == value)
					{
						if (j == 0)
						{

						}
						else if (j > 0 && _currentLevel->getvalue(j-1,i) == -1 && (_currentLevel->gettype(j - 1, i) == nothing || _currentLevel->gettype(j - 1, i) == guard))
						{
							_currentLevel->setvalue(j - 1, i, value + 1);// _levalues[j - 1][i].value = value + 1;
							maxValue--;
						}

						if (j == _height - 1)
						{

						}
						else if (j < _height &&  _currentLevel->getvalue(j + 1, i) == -1 && (_currentLevel->gettype(j + 1, i) == nothing || _currentLevel->gettype(j + 1, i) == guard))
						{
							_currentLevel->setvalue(j + 1, i, value + 1);// _levalues[j + 1][i].value = value + 1;
							maxValue--;
						}

						if (i == 0)
						{

						}
						else if (i > 0 && _currentLevel->getvalue(j, i - 1) == -1 && (_currentLevel->gettype(j, i - 1) == nothing || _currentLevel->gettype(j, i - 1) == guard))
						{
							_currentLevel->setvalue(j, i-1, value + 1); //_levalues[j][i - 1].value = value + 1;
							maxValue--;
						}

						if (i == _width - 1)
						{

						}
						else if (i < _width && _currentLevel->getvalue(j, i + 1) == -1 && (_currentLevel->gettype(j, i + 1) == nothing || _currentLevel->gettype(j, i + 1) == guard))
						{
							_currentLevel->setvalue(j, i + 1, value + 1);//	_levalues[j][i + 1].value = value + 1;
							maxValue--;
						}
					}
				}
			}
		}
		if (oldMaxValue == maxValue)
		{
			break;
		}
		value++;
	}
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			if (_currentLevel->gettype(j,i) == wall)
			{
				_currentLevel->setvalue(j, i, -2);//_levalues[j][i].value = -2;
			}
		}
	}

	std::vector<glm::vec2> path;

	glm::ivec2 currentPos = goalPos;
	if(_currentLevel->getvalue(goalPos.y, goalPos.x)>0)//_levalues[goalPos.y][goalPos.x].value>0)
	{
	path.push_back(goalPos);
	}
	glm::ivec2 startPos = glm::ivec2(_width / 2, _height / 2);


	int currentValue = (_currentLevel->getvalue(goalPos.y, goalPos.x));//_levalues[goalPos.y][goalPos.x].value;
	while (currentValue > 1)
	{
		if (_currentLevel->getvalue(currentPos.y - 1, currentPos.x)<currentValue && _currentLevel->getvalue(currentPos.y - 1, currentPos.x) >= 0)//_levalues[currentPos.y - 1][currentPos.x].value < currentValue && _levalues[currentPos.y - 1][currentPos.x].value >= 0)
		{
			currentPos.y -= 1;
			path.push_back(currentPos);
		}
		else if (_currentLevel->getvalue(currentPos.y + 1, currentPos.x)<currentValue && _currentLevel->getvalue(currentPos.y + 1, currentPos.x) >= 0)//_levalues[currentPos.y + 1][currentPos.x].value < currentValue && _levalues[currentPos.y + 1][currentPos.x].value >= 0)
		{
			currentPos.y += 1;
			path.push_back(currentPos);
		}
		else if (_currentLevel->getvalue(currentPos.y, currentPos.x - 1)<currentValue && _currentLevel->getvalue(currentPos.y, currentPos.x - 1) >= 0)//_levalues[currentPos.y][currentPos.x - 1].value < currentValue && _levalues[currentPos.y][currentPos.x - 1].value >= 0)
		{
			currentPos.x -= 1;
			path.push_back(currentPos);
		}
		else if (_currentLevel->getvalue(currentPos.y, currentPos.x + 1)<currentValue && _currentLevel->getvalue(currentPos.y, currentPos.x + 1) >= 0)//_levalues[currentPos.y][currentPos.x + 1].value < currentValue && _levalues[currentPos.y][currentPos.x + 1].value >= 0)
		{
			currentPos.x += 1;
			path.push_back(currentPos);
		}
		currentValue--;
	}
	
	std::vector<glm::vec2> pathdisplacement;
	glm::vec2 displacementForVecTwo = glm::vec2(_displacement.x, _displacement.z);
	for (unsigned int i = 0; i < path.size(); i++)
	{
		path[i] = path[i]+displacementForVecTwo;
	}
	return path;
}

bool Guard::walkingInThePaths(float dt)
{
	if (_currentPath.size() == 0)
	{
		return true;
	}
	
	glm::vec2 roundedPosition = glm::vec2(this->getWorldPos().x, this->getWorldPos().z);
	goToSquare(dt, _currentPath[_currentPath.size() - 1]);
	
	if (roundedPosition == _currentPath[_currentPath.size() - 1])
	{
		if (_currentPath.size() > 1)
		{
			glm::vec2 currentSquare = _currentPath[_currentPath.size() - 1];
			glm::vec2 nextSquare = _currentPath[_currentPath.size() - 2];

			glm::vec2 difference = (currentSquare - nextSquare) * -1.f;
			difference = glm::normalize(difference);

			_forward = glm::vec3(difference.x, 0.0f, difference.y);

			float angle = acos(_forward.z);

			if ( _forward.x < 0.f)
			{
				angle = M_2PIf - angle;
			}

			_currentRot = angle;

			setRX(_currentRot);
		}
		_currentPath.erase(_currentPath.begin() + _currentPath.size() - 1);
	}
	return false;
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
		/*	case -2:
		{

		}*/
		case 3:
		{
			goToSquare(dt, _point2x);
			if (roundedPosition == glm::vec2(_point2x.x, _point2x.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		/*case -3:
		{

		}*/
		case 4:
		{
			goToSquare(dt, _point1x);
			if (roundedPosition == glm::vec2(_point1x.x, _point1x.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		/*case -4:
		{

		}*/
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
	if (walkingInThePaths(dt))
	{
		_currentPath = generatingPath(glm::ivec2(randomgenerator(_width)-1,randomgenerator(_height)-1));
	}

	if (glm::length(this->getWorldPos() - _player->getWorldPos()) < GUARDVIEWDISTANCE)
	{
		if (DetectedPlayer())
		{
			std::cout << "Detected player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			//GameOverEvent event(false);
			//_eventManager->execute(event);
		}
	}

	GameObject::update(dt);
}

Guard::Guard(Character* player, EventManager* event, Model &m, Grid *gridet) :
	GameObject(m), _player(player), _eventManager(event), _displacement(0.5f, 0.f, 0.5f)
{
	_forward = glm::vec3(0.f, 0.f, -1.f);
	glm::vec3 guardStartPosition=gridet->getLastValueOfGuardLocationsAndremovesit();
	srand((unsigned int)time(NULL));
	//x = h�jd z= bred
	gridet->Creategetheightandwidthpoint12(guardStartPosition);
	_point1z = gridet->getheightandwidthpoint12(0)+ _displacement;
	_point2z = gridet->getheightandwidthpoint12(1)+ _displacement;
	_point1x = gridet->getheightandwidthpoint12(2)+ _displacement;
	_point2x = gridet->getheightandwidthpoint12(3)+ _displacement;

	_displacement = glm::vec3(0.5f, 0.0f, 0.5f);

	_guardsstartposition = guardStartPosition + _displacement;
	_widthLength = gridet->getWidth();
	_heightLength = gridet->getHeight();
	
	this->setPosition(_guardsstartposition);	

//	_currentLevel = gridet->getGrid();

	_speed = 0.4f;

	_aiChoice = randomgenerator(4);
//	buildgridarray(gridet, 10, 20);
//	_currentPath = generatingPath(glm::ivec2(randomgenerator(_widthLength) - 1, randomgenerator(_heightLength) - 1)); //ändra till random efteråt
	//	std::cout<< guardsstartposition.x<<" "<< guardsstartposition.y << " "<<guardsstartposition.z<< std::endl;
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
	//walk up to top point
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

void Guard::goToSquare(float dt, glm::vec2 walkToSquare)
{
	return;
	glm::vec3 walkTo = glm::vec3(walkToSquare.x, 0, walkToSquare.y);
	glm::vec3 value = this->getPosition();
	glm::vec3 distance = walkTo - value;
	distance = glm::vec3(distance.x, 0, distance.z);
	_distLength = glm::length(distance);
	
	if (distance.z > 0)
	{
		if (_distLength < (_speed*dt))
		{
			this->setPosition(value.x, value.y, walkTo.z);

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
		if (_distLength < (_speed*dt))
		{
			this->setPosition(walkTo.x, value.y, value.z);

		}
		else
		{
			this->move(glm::vec3(_speed, 0, 0) * dt);
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

	pos.y = 1.3f;

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
		float wallDist = _currentLevel->getWallDist(pos, rayUpperLeft, GUARDVIEWDISTANCE);
		float objectDist = _currentLevel->getObjectDist(pos, rayLowerLeft, GUARDVIEWDISTANCE, playerPos);

		if (playerDist < wallDist || playerDist < objectDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && objectDist == 0.0f && playerDist < (GUARDVIEWDISTANCE * 1.0f))
		{
			result = true;
		}

		wallDist =_currentLevel->getWallDist(pos, rayUpperRight, GUARDVIEWDISTANCE);
		objectDist = _currentLevel->getObjectDist(pos, rayLowerLeft, GUARDVIEWDISTANCE, playerPos);

		if (playerDist < wallDist || playerDist < objectDist)
		{
			result = true;
		}

		else if (wallDist == 0.0f && objectDist == 0.0f && playerDist < (GUARDVIEWDISTANCE * 1.0f))
		{
			result = true;
		}
	}

	return result;
}