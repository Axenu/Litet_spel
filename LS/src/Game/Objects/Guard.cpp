#include "Game/Objects/Guard.h"

Guard::~Guard()
{
	for (int j = 0; j < _height; j++)
	{
		delete[] _levalues[j];
	}
	delete[] _levalues;
}

Guard::Guard()
{

}

void Guard::print()
{
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			if (_levalues[j][i].value == -1 || _levalues[j][i].value == -2)
			{
				std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_levalues[j][i].value << " ";
			}
			else if (_levalues[j][i].value >=0 && _levalues[j][i].value < 10)
			{
				std::cout << "+" << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_levalues[j][i].value << " ";
			}
			else
				std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_levalues[j][i].value << " ";
		}
		std::cout << "" << std::endl;
	}
	std::cout << "end" << std::endl;
}

std::vector<glm::ivec2> Guard::generatingPath(glm::ivec2 GoalPosition)
{
	int maxvalue = _height * _width - 1;
	int oldmaxvalue = 0;
	int value = 0;
	glm::ivec2 goalPos = glm::ivec2((int)GoalPosition.x, (int)GoalPosition.y);
	//-1 outforskadmark ej gåbart
	// -2 vägg
	while (maxvalue != 0)
	{
		oldmaxvalue = maxvalue;
		for (int j = 0; j < _height; j++)
		{
			for (int i = 0; i < _width; i++)
			{
				if (_levalues[j][i].type == nothing || _levalues[j][i].type == guard)
				{
					if (_levalues[j][i].value == value)
					{
						if (j == 0)
						{

						}
						else if (j > 0 && _levalues[j - 1][i].value == -1 && (_levalues[j - 1][i].type == nothing || _levalues[j - 1][i].type == guard))
						{
							_levalues[j - 1][i].value = value + 1;
							maxvalue--;
						}

						if (j == _height - 1)
						{

						}
						else if (j < _height && _levalues[j + 1][i].value == -1 && (_levalues[j + 1][i].type == nothing || _levalues[j + 1][i].type == guard))
						{
							_levalues[j + 1][i].value = value + 1;
							maxvalue--;
						}

						if (i == 0)
						{

						}
						else if (i > 0 && _levalues[j][i - 1].value == -1 && (_levalues[j][i - 1].type == nothing || _levalues[j][i - 1].type == guard))
						{
							_levalues[j][i - 1].value = value + 1;
							maxvalue--;
						}


						if (i == _width - 1)
						{

						}
						else if (i < _width && _levalues[j][i + 1].value == -1 && (_levalues[j][i + 1].type == nothing || _levalues[j][i + 1].type == guard))
						{
							_levalues[j][i + 1].value = value + 1;
							maxvalue--;
						}
					}
				}
			}
		}
		if (oldmaxvalue == maxvalue)
		{
			break;
		}
		value++;
	}
	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			if (_levalues[j][i].type == wall)
			{
				_levalues[j][i].value = -2;
			}
		}
	}
	print();

	std::vector<glm::ivec2> path;

	glm::ivec2 currentPos = goalPos;
	path.push_back(goalPos);

	glm::ivec2 startPos = glm::ivec2(_width / 2, _height / 2);

	int currentValue = _levalues[goalPos.y][goalPos.x].value;
	while (currentValue > 1)
	{
		if (_levalues[currentPos.y - 1][currentPos.x].value < currentValue && _levalues[currentPos.y - 1][currentPos.x].value >= 0)
		{
			currentPos.y -= 1;
			path.push_back(currentPos);
		}
		else if (_levalues[currentPos.y + 1][currentPos.x].value < currentValue && _levalues[currentPos.y + 1][currentPos.x].value >= 0)
		{
			currentPos.y += 1;
			path.push_back(currentPos);
		}
		else if (_levalues[currentPos.y][currentPos.x - 1].value < currentValue && _levalues[currentPos.y][currentPos.x - 1].value >= 0)
		{
			currentPos.x -= 1;
			path.push_back(currentPos);
		}
		else if (_levalues[currentPos.y][currentPos.x + 1].value < currentValue && _levalues[currentPos.y][currentPos.x + 1].value >= 0)
		{
			currentPos.x += 1;
			path.push_back(currentPos);
		}
		currentValue--;
	}

	for (unsigned int i = 0; i < path.size(); i++)
	{
		path[i] = _levalues[path[i].y][path[i].x].xz;
	}
	return path;
}

void Guard::walkingInThePaths(float dt)
{
	if (_currentPath.size() == 0)
	{
		return;
	}
	glm::ivec2 roundedpositon = roundTheValuefrom0Comma01(this->getWorldPos());
	goToSquare(dt, _currentPath[_currentPath.size() - 1]);
	std::cout << "(" << this->getWorldPos().x << ", " << this->getWorldPos().z << "), (" << roundedpositon.x << ", " << roundedpositon.y << "), (" << _currentPath[_currentPath.size() - 1].x << ", " << _currentPath[_currentPath.size() - 1].y << ")" << std::endl;
	if (roundedpositon == _currentPath[_currentPath.size() - 1])
	{
		_currentPath.erase(_currentPath.begin() + _currentPath.size() - 1);
	}
}

glm::ivec2 Guard::roundTheValuefrom0Comma01(glm::vec3 normalvalue)
{
	glm::ivec2 returnValue = glm::ivec2((int)normalvalue.x, (int)normalvalue.z);
	if (normalvalue.x < round(normalvalue.x) - 0.01f)
	{
		returnValue.x = int(round(normalvalue.x));
	}
	if (normalvalue.z < round(normalvalue.z) - 0.01f)
	{
		returnValue.y = int(round(normalvalue.z));
	}
	return returnValue;
}
Guard::Guard(Character* player, EventManager* event, glm::vec3 Guarden, glm::vec3 positonxy[4]) :
	GameObject(), _player(player), _eventManager(event)
{
	this->setPosition(Guarden);
	_point1x = positonxy[0];
	_point2x = positonxy[1];
	_point1z = positonxy[2];
	_point2z = positonxy[3];
}

void Guard::WalkingBetweenFourPoints(float dt)
{
	glm::ivec2 roundedpositon;
	roundedpositon = roundTheValuefrom0Comma01(this->getPosition());

	switch (_aiChoice)
	{
		case 1:
		{
			goToSquare(dt, _point1z);
			if (roundedpositon == glm::ivec2(_point1z.x, _point1z.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		case -1:
		{
			goToSquare(dt, _guardsstartposition);
			if (roundedpositon == glm::ivec2(_guardsstartposition.x, _guardsstartposition.z))
			{
				_aiChoice = randomgenerator(4);
			}
			break;
		}
		case 2:
		{
			goToSquare(dt, _point2z);
			if (roundedpositon == glm::ivec2(_point2z.x, _point2z.z))
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
			if (roundedpositon == glm::ivec2(_point2x.x, _point2x.z))
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
			if (roundedpositon == glm::ivec2(_point1x.x, _point1x.z))
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

void Guard::buildgridarray(Grid * gridet, unsigned int sizeX, unsigned int sizeY)
{
	_currentGridSpace = gridet->getGridSpace();
	unsigned int maxsizewidth = sizeX;
	_height = 1, _width = 1;

	for (unsigned int i = maxsizewidth; i > 0; i--)
	{
		if (_guardsstartposition.x >= i && _guardsstartposition.x + i <= _widthLength)
		{
			_width = i * 2;
			break;
		}
	}
	int maxsizeheight = sizeY;
	for (unsigned int j = maxsizeheight; j > 0; j--)
	{
		if (_guardsstartposition.z >= j && _guardsstartposition.z + j <= _heightLength)
		{
			_height = j * 2;
			break;
		}
	}

	//building the 2D array
	_levalues = new gridValues*[_height];
	for (int j = 0; j < _height; j++)
	{
		_levalues[j] = new gridValues[_width];
	}

	for (int j = 0; j < _height; j++)
	{
		for (int i = 0; i < _width; i++)
		{
			_levalues[j][i].type = gridet->returnGridType((int)_guardsstartposition.x + i - (_width / 2), (int)_guardsstartposition.z + j - (_height / 2));
			_levalues[j][i].xz = glm::vec2((int)_guardsstartposition.x + i - (_width / 2), (int)_guardsstartposition.z + j - (_height / 2));
			if (_levalues[j][i].type == guard)
			{
				_levalues[j][i].value = 0;
			}
			else
			{
				_levalues[j][i].value = -1;
			}
			//std::cout << levalues[i][j].xz.x << levalues[i][j].xz.y << std::endl;
		}
	}
//	print();
}

void Guard::update(float dt)
{

	walkingInThePaths(dt);
//	WalkingBetweenFourPoints(dt);

	if (this->DetectedPlayer())
	{
		GameOverEvent event(false);
		_eventManager->execute(event);
	}

	Node::update(dt);
}

Guard::Guard(Character* player, EventManager* event, Model &m, Grid *gridet) :
	GameObject(m), _player(player), _eventManager(event)
{
	srand((unsigned int)time(NULL));
	//x = h�jd z= bred
	gridet->Creategetheightandwidthpoint12(gridet->getData(guard));
	_point1z = gridet->getheightandwidthpoint12(0);
	_point2z = gridet->getheightandwidthpoint12(1);
	_point1x = gridet->getheightandwidthpoint12(2);
	_point2x = gridet->getheightandwidthpoint12(3);

	_guardsstartposition = gridet->getData(guard);
	_widthLength = gridet->getWidth();
	_heightLength = gridet->getHeight();
	this->setPosition(_guardsstartposition);

	_aiChoice = randomgenerator(4);
	buildgridarray(gridet, 10, 20);
	_currentPath = generatingPath(glm::ivec2(10, 5));
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
	float speed = 0.3f;
	//float padding = 0.f;
	glm::vec3 value = this->getPosition();

	//glm::vec3 value = glm::vec3(1, 0, 5);
	glm::vec3 distance = walkTo - value;
	//	std::cout << distance.x << " " << distance.z << std::endl;
	if (distance.z > 0)
	{
		this->move(glm::vec3(0, 0, speed) * dt);
	}
	if (distance.z < 0)
	{
		this->move(glm::vec3(0, 0, -speed) * dt);
	}
	if (distance.x > 0)
	{
		this->move(glm::vec3(speed, 0,0) * dt);
	}
	if (distance.x < 0)
	{
		this->move(glm::vec3(-speed, 0, 0) * dt);
	}
}

void Guard::goToSquare(float dt, glm::ivec2 walkTo)
{
	//walk up to top point
	float speed = 0.3f;
	//float padding = 0.f;
	glm::vec3 value = this->getWorldPos();

	//glm::vec3 value = glm::vec3(1, 0, 5);
	glm::ivec2 distance = walkTo - glm::ivec2(value.x, value.z);
	//	std::cout << distance.x << " " << distance.z << std::endl;
	if (distance.y > 0)
	{
		this->move(glm::vec3(0, 0, speed) * dt);
	}
	if (distance.y < 0)
	{
		this->move(glm::vec3(0, 0, -speed) * dt);
	}
	if (distance.x > 0)
	{
		this->move(glm::vec3(speed, 0, 0) * dt);
	}
	if (distance.x < 0)
	{
		this->move(glm::vec3(-speed, 0, 0) * dt);
	}
}

bool Guard::DetectedPlayer()
{
	float distance = getDistance(*_player);

	if (distance > 2.0f)
	{
		return false;
	}

	return true;
}
