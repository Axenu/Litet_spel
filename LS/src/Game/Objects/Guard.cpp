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

void Guard::buildgridarray()
{
	 height=1, width=1;
	if (guardsstartposition.x >= 5 && guardsstartposition.x+5<=_heightLength)
	{
		height = 5;
	}
	else if(guardsstartposition.x+guardsstartposition.x<=_heightLength)
	{
		height = guardsstartposition.x+1;
	}
	else
	{
		height=_heightLength - guardsstartposition.x;
	}



	//building the 2D array
	levalues = new gridValues*[height];
	for (int i = 0; i < height; i++)
	{
		levalues[i] = new gridValues[width];
	}
}



void Guard::update(float dt)
{
	WalkingBetweenFourPoints(dt);
	Node::update(dt);
}

Guard::Guard(Model &m, Grid *gridet):
	GameObject(m)
{
	srand(time(NULL));
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
