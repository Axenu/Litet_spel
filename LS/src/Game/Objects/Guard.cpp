#include "Game/Objects/Guard.h"

Guard::~Guard()
{
	for (int i = 0; i < _height; i++)
	{
		delete[] _levalues[i];
	}

	delete[] _levalues;
}

Guard::Guard()
{

}
void Guard::print()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_levalues[i][j].value == -1 || _levalues[i][j].value == -2)
			{
				std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_levalues[i][j].value << " ";
			}
			else
			{
				std::cout << "+" << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/_levalues[i][j].value << " ";
			}
		}
		std::cout << "" << std::endl;
	}
	std::cout << "end" << std::endl;
}
void Guard::gridWalkingBetweenTwoPoints(glm::vec3 GoalPosition)
{
	int maxvalue = _height * _width - 1;
	int oldmaxvalue = 0;
	int value = 0;
	int heightpositionofthegoal = (int)GoalPosition.x;
	int widthpositionofthegoal = (int)GoalPosition.z;
	//-1 outforskadmark ej gåbart
	// -2 vägg
	while (maxvalue != 0)
	{
		oldmaxvalue = maxvalue;
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				if (_levalues[i][j].type == nothing || _levalues[i][j].type == guard)
				{
					if (_levalues[i][j].value == value)
					{
						if (i == 0)
						{

						}
						else if (i > 0 && _levalues[i - 1][j].value == -1 && (_levalues[i - 1][j].type == nothing || _levalues[i - 1][j].type == guard))
						{
							_levalues[i-1][j].value = value + 1;
							maxvalue--;
						}
						if (i == _height - 1)
						{

						}
						else if (i < _height && _levalues[i + 1][j].value == -1 && (_levalues[i + 1][j].type == nothing || _levalues[i + 1][j].type == guard))
						{
							_levalues[i + 1][j].value = value + 1;
							maxvalue--;
						}
					
					
						if (j == 0)
						{

						}
						else if (j > 0 && _levalues[i][j -1].value == -1 && (_levalues[i][j -1].type == nothing || _levalues[i][j - 1].type == guard))
						{
							_levalues[i][j - 1].value = value + 1;
							maxvalue--;
						}


						if (j == _width - 1)
						{

						}
						else if (j < _width && _levalues[i][j + 1].value == -1 && (_levalues[i][j + 1].type == nothing || _levalues[i][j + 1].type == guard))
						{
							_levalues[i][j + 1].value = value + 1;
							maxvalue--;
						}
					}
				}
			}
		}
		//print();
		if (oldmaxvalue == maxvalue)
		{
			break;
		}
		value++;
	}

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			if (_levalues[i][j].type == wall)
			{
				_levalues[i][j].value = -2;
			}
		}
	}
}
glm::vec3 Guard::roundTheValuefrom0Comma01(glm::vec3 normalvalue)
{
	glm::vec3 returnvalue = normalvalue;
	if (normalvalue.x < round(normalvalue.x) + 0.1f)
	{
		returnvalue.x = round(normalvalue.x);
	}
	if (normalvalue.z < round(normalvalue.z) + 0.1f)
	{
		returnvalue.z = round(normalvalue.z);
	}
	if (normalvalue.y < round(normalvalue.y) + 0.1f)
	{
		returnvalue.y = round(normalvalue.y);
	}
	return returnvalue;
}
Guard::Guard(glm::vec3 Guarden, glm::vec3 positonxy[4]) : GameObject()
{
	this->setPosition(Guarden);
	_point1x = positonxy[0];
	_point2x = positonxy[1];
	_point1z = positonxy[2];
	_point2z = positonxy[3];
}

void Guard::WalkingBetweenFourPoints(float dt)
{
	glm::vec3 roundedpositon;
	roundedpositon = roundTheValuefrom0Comma01(this->getPosition());

	switch (_aiChoice)
	{
		case 1:
		{
			goToSquare(dt, _point1z);
			if (roundedpositon == glm::vec3(_point1z.x, roundedpositon.y, _point1z.z))
			{
				_aiChoice = -1;
			}
			break;
		}
		case -1:
		{
			goToSquare(dt, _guardsstartposition);
			if (roundedpositon == glm::vec3(_guardsstartposition.x, roundedpositon.y, _guardsstartposition.z))
			{
				_aiChoice = randomgenerator();
			}
			break;
		}
		case 2:
		{
			goToSquare(dt, _point2z);
			if (roundedpositon == glm::vec3(_point2z.x, roundedpositon.y, _point2z.z))
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
			if (roundedpositon == glm::vec3(_point2x.x, roundedpositon.y, _point2x.z))
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
			if (roundedpositon == glm::vec3(_point1x.x, roundedpositon.y, _point1x.z))
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

void Guard::buildgridarray(Grid * gridet)
{
	int maxsizewidth = 6;
	_height = 1, _width = 1;
	
	for(int i = maxsizewidth; i > 0; i--)
	{
		if (_guardsstartposition.x >= i && _guardsstartposition.x+i<=_heightLength)
		{
			_height = i * 2;
			break;
		}
	}
	int maxsizeheight = 6;
	for (int i = maxsizeheight; i > 0; i--)
	{
		if (_guardsstartposition.z >= i && _guardsstartposition.z + i <= _widthLength)
		{
			_width = i * 2;
			break;
		}
	}

	//building the 2D array
	_levalues = new gridValues*[_height];
	for (int i = 0; i < _height; i++)
	{
		_levalues[i] = new gridValues[_width];
	}
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j  < _width; j++)
		{
			_levalues[i][j].type = gridet->returnGridType((int)_guardsstartposition.x + i - (_height / 2), (int)_guardsstartposition.z + j - (_width / 2));
			_levalues[i][j].xz = glm::vec2((int)_guardsstartposition.x + i - (_height / 2), (int)_guardsstartposition.z + j - (_width / 2));
			if (_levalues[i][j].type == guard)
			{
				_levalues[i][j].value = 0;
			}
			else
			{
				_levalues[i][j].value = -1;
			}
			//std::cout << levalues[i][j].xz.x << levalues[i][j].xz.y << std::endl;
		}
	}
	print();
}



void Guard::update(float dt)
{
	WalkingBetweenFourPoints(dt);
	Node::update(dt);
}

Guard::Guard(Model &m, Grid *gridet):
	GameObject(m)
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
	
	_aiChoice = randomgenerator();
	buildgridarray(gridet);
	gridWalkingBetweenTwoPoints(glm::vec3(5, 0, 6));
	//	std::cout<< guardsstartposition.x<<" "<< guardsstartposition.y << " "<<guardsstartposition.z<< std::endl;
}

int Guard::randomgenerator()
{
	return (int)rand() % 4 + 1;
}

void Guard::setPositionfromMap(glm::vec3 Guarden)
{
	this->setPosition(Guarden);

}

void Guard::goToSquare(float dt,glm::vec3 walkTo)
{
	//walk up to top point
	float speed = 0.3f;
	float padding = 0.f;
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

bool Guard::DetectedPlayer(glm::vec3 player)
{
	float distance = getDistance(glm::vec4(player, 1.0f));

	if (distance > 1.0f)
	{
		return false;
	}

	return true;
}
