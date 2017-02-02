#include "Guard.h"

Guard::~Guard()
{
}

Guard::Guard()
{

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
//std::cout << this->getPosition().z << std::endl;
	//walk up to top point
	float speed = 0.03f;
	float padding = 0.f;
float zvalue=	this->getPosition().z;
	if (aiChoice == 1)
	{
		if(zvalue - padding <= point2z.z)
		{ 
			aiChoice = -1;
		}
		else
		this->move(glm::vec3(0,0,-speed)*dt);
	}
	//walk down to the guardstartpoint
	if (aiChoice == -1)
	{
		if (guardsstartposition.z <= zvalue)
		{
			aiChoice = 2; // add randomness later
		}
		else
		this->move(glm::vec3(0, 0, speed)*dt);

	}

	//walk down to lowest point
	if (aiChoice == 2)
	{
		if (zvalue  >= point1z.z)
		{
			aiChoice = -2;
		}
		else
			this->move(glm::vec3(0, 0, speed)*dt);
	}
	//walk up to the guardstartpoint
	if (aiChoice == -2)
	{
		if (guardsstartposition.z >= zvalue)
		{
			aiChoice = 0; // add randomness later
		}
		else
		this->move(glm::vec3(0, 0, -speed)*dt);

	}

}




void Guard::onUpdate(float dt)
{

	WalkingBetweenFourPoints(dt);
}

Guard::Guard(Model &m, Grid *gridet):
	GameObject(m)
{
	gridet->Createxandypoint12(glm::vec3(gridet->getData(guard).xz.x, 0, gridet->getData(guard).xz.y));
	point1z = gridet->getxandypoint12(0);
	point2z = gridet->getxandypoint12(1);
	point1x = gridet->getxandypoint12(2);
	point2x = gridet->getxandypoint12(3);
	guardsstartposition = glm::vec3(gridet->getData(guard).xz.x+0.01, 0.01, gridet->getData(guard).xz.y+0.01);
	this->setPosition(guardsstartposition);
	aiChoice = 0;
//	std::cout<< guardsstartposition.x<<" "<< guardsstartposition.y << " "<<guardsstartposition.z<< std::endl;
}

void Guard::setPositionfromMap(glm::vec3 Guarden)
{
	this->setPosition(Guarden);

}



