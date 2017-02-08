#include "Game/Objects/Guard.h"

Guard::~Guard()
{
	for (int i = 0; i < height; i++)
	{
		delete[] levalues[i];
	}

	delete[] levalues;
}

Guard::Guard()
{

}
void Guard::print()
{

	for (int i = 0;i < height;i++)
	{
		for (int j = 0;j < width;j++)
		{
			if(levalues[i][j].value== -1 || levalues[i][j].value == -2)
			std::cout << /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/levalues[i][j].value<<" "; 
			else
			std::cout <<"+"<< /*levalues[i][j].xz.x<<","<< levalues[i][j].xz.y<<"."<<*/levalues[i][j].value << " ";
		}
		std::cout<<""<<std::endl;
	}
	std::cout << "end" << std::endl;

}
void Guard::gridWalkingBetweenTwoPoints(glm::vec3 GoalPosition)
{
int maxvalue = height*width-1;
int oldmaxvalue = 0;
int value = 0;
int heightpositionofthegoal = (int)GoalPosition.x;
int widthpositionofthegoal = (int)GoalPosition.z;
//-1 outforskadmark ej gåbart
// -2 vägg
while (maxvalue != 0)
{
	oldmaxvalue = maxvalue;
	for (int i = 0;i < height;i++)
	{
		for (int j = 0;j < width;j++)
		{

			if(i ==3 && j ==6)
			{
			//	std::cout << "swag" << std::endl;
			}
			if(levalues[i][j].type == nothing || levalues[i][j].type == guard)
			{
				if (levalues[i][j].value == value)
				{

					if (i == 0)
					{

					}
					else if (i > 0 && levalues[i - 1][j].value == -1 && (levalues[i - 1][j].type == nothing || levalues[i - 1][j].type == guard))
					{
						levalues[i-1][j].value = value + 1;
						maxvalue--;
					}
					if (i == height-1)
					{

					}
					else if (i < height && levalues[i + 1][j].value == -1 && (levalues[i + 1][j].type == nothing || levalues[i + 1][j].type == guard))
					{
						levalues[i + 1][j].value = value + 1;
						maxvalue--;
					}
					
					
					if (j == 0)
					{

					}
					else if (j > 0 && levalues[i][j -1].value == - 1 && (levalues[i][j -1].type == nothing || levalues[i][j - 1].type == guard))
					{
						levalues[i ][j - 1].value = value + 1;
						maxvalue--;
					}


					if (j == width-1)
					{

					}
					else if (j < width&& levalues[i][j + 1].value == -1 && (levalues[i][j + 1].type == nothing || levalues[i][j + 1].type == guard))
					{
						levalues[i][j + 1].value = value + 1;
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

for (int i = 0;i < height;i++)
{
	for (int j = 0;j < width;j++)
	{
		if (levalues[i][j].type == wall)
			levalues[i][j].value = -2;
	}
}

//first check down 
int newvalue = 1000000000;
int smallestvalue = 1000000000;
if (GoalPosition.x > 0)
{
	newvalue = smallestvalue=levalues[heightpositionofthegoal-1][widthpositionofthegoal].value;
}
//secondcheck upwards
if (GoalPosition.x < height)
{
	if(smallestvalue>newvalue)
	{
	newvalue = smallestvalue = levalues[heightpositionofthegoal + 1][widthpositionofthegoal].value;
	}
}
//checking leftvalue
if (GoalPosition.z > 0)
{
	if (smallestvalue > newvalue)
	{
		newvalue = smallestvalue = levalues[heightpositionofthegoal][widthpositionofthegoal - 1].value;
	}
}
//checking rightvalue
if (GoalPosition.z < width)
{
	if (smallestvalue > newvalue)
	{
		newvalue = smallestvalue = levalues[heightpositionofthegoal][widthpositionofthegoal + 1].value;
	}
}
//print();
}
glm::vec3 Guard::roundTheValuefrom0Comma01(glm::vec3 normalvalue)
{
	glm::vec3 returnvalue = normalvalue;
	if (normalvalue.x < round(normalvalue.x) + 0.1)
	{
		returnvalue.x = round(normalvalue.x);
	}
	if (normalvalue.z < round(normalvalue.z) + 0.1)
	{
		returnvalue.z = round(normalvalue.z);
	}
	if (normalvalue.y < round(normalvalue.y) + 0.1)
	{
		returnvalue.y = round(normalvalue.y);
	}
	return returnvalue;
}
Guard::Guard(glm::vec3 Guarden,glm::vec3 positonxy[4]):
GameObject()
{
	this->setPosition(Guarden);
	 point1x = positonxy[0];
	 point2x = positonxy[1];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
	 point1z = positonxy[2];
	 point2z = positonxy[3];

}



void Guard::WalkingBetweenFourPoints(float dt)
{
	glm::vec3 roundedpositon;
	roundedpositon = roundTheValuefrom0Comma01(this->getPosition());

	switch (aiChoice)
	{
	case 1:
	{
		goToSquare(dt, point1z);
		if (roundedpositon == glm::vec3(point1z.x, roundedpositon.y, point1z.z))
		{
			aiChoice = -1;
		}
		break;
	}
	case -1:
	{
		goToSquare(dt, guardsstartposition);
		if (roundedpositon == glm::vec3(guardsstartposition.x, roundedpositon.y, guardsstartposition.z))
		{
			aiChoice = randomgenerator();
		}
		break;
	}
	case 2:
	{
		goToSquare(dt, point2z);
		if (roundedpositon == glm::vec3(point2z.x, roundedpositon.y,point2z.z))
		{
			aiChoice = -1;
		}
		break;
	}
	case -2:
	{

	}
	case 3:
	{
		goToSquare(dt, point2x);
		if (roundedpositon == glm::vec3(point2x.x, roundedpositon.y, point2x.z))
		{
			aiChoice = -1;
		}
		break;
	}
	case -3:
	{

	}
	case 4:
	{
		goToSquare(dt, point1x);
		if (roundedpositon == glm::vec3(point1x.x, roundedpositon.y, point1x.z))
		{
			aiChoice = -1;
		}
		break;
	}
	case -4:
	{

	}
	default:
		std::cout << "something went horribley wrong here AI HAS SOME FLAWS MATE" << std::endl;

}






}

void Guard::buildgridarray(Grid * gridet)
{
	int maxsizewidth = 6;
	height = 1, width = 1;
	for (int i = maxsizewidth; i > 0; i--)
	{
		if (guardsstartposition.x >= i && guardsstartposition.x + i <= _heightLength)
		{
			height = i * 2;
			break;
		}
	}
	int maxsizeheight = 6;
	for (int i = maxsizeheight; i > 0; i--)
	{
		if (guardsstartposition.z >= i && guardsstartposition.z + i <= _widthLength)
		{
			width = i * 2;
			break;
		}
	}

	//building the 2D array
	levalues = new gridValues*[height];
	for (int i = 0; i < height; i++)
	{
		levalues[i] = new gridValues[width];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			levalues[i][j].type = gridet->returnGridType(guardsstartposition.x + i - (height / 2), guardsstartposition.z + j - (width / 2));
			levalues[i][j].xz = glm::vec2(guardsstartposition.x + i - (height / 2), guardsstartposition.z + j - (width / 2));
			if (levalues[i][j].type == guard)
			{
				levalues[i][j].value = 0;
			}
			else
			{
				levalues[i][j].value = -1;
			}
			//		std::cout << levalues[i][j].xz.x << levalues[i][j].xz.y << std::endl;
		}
	}
	//	print();
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
	point1z = gridet->getheightandwidthpoint12(0);
	point2z = gridet->getheightandwidthpoint12(1);
	point1x = gridet->getheightandwidthpoint12(2);
	point2x = gridet->getheightandwidthpoint12(3);
	guardsstartposition = gridet->getData(guard);
	_widthLength = gridet->getWidth();
	_heightLength = gridet->getHeight();
	this->setPosition(guardsstartposition);
	aiChoice = randomgenerator();
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
		this->move(glm::vec3(0, 0, speed)*dt);
	}
	if (distance.z < 0)
	{
		this->move(glm::vec3(0, 0, -speed)*dt);
	}
	if (distance.x > 0)
	{
		this->move(glm::vec3(speed, 0,0)*dt);
	}
	if (distance.x < 0)
	{
		this->move(glm::vec3(-speed, 0, 0)*dt);
	}
}
