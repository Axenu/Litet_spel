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


}




void Guard::update(float dt)
{

//	WalkingBetweenFourPoints(dt);
	goToSquare(dt, glm::vec3(1,0,5));
	Node::update(dt);
}

Guard::Guard(Model &m, Grid *gridet):
	GameObject(m)
{
	//x = höjd z= bred
	gridet->Creategetheightandwidthpoint12(gridet->getData(guard));
	point1z = gridet->getheightandwidthpoint12(0);
	point2z = gridet->getheightandwidthpoint12(1);
	point1x = gridet->getheightandwidthpoint12(2);
	point2x = gridet->getheightandwidthpoint12(3);
	guardsstartposition = gridet->getData(guard);
	this->setPosition(guardsstartposition);
	aiChoice = 1;
//	std::cout<< guardsstartposition.x<<" "<< guardsstartposition.y << " "<<guardsstartposition.z<< std::endl;
}

void Guard::setPositionfromMap(glm::vec3 Guarden)
{
	this->setPosition(Guarden);

}

void Guard::goToSquare(float dt,glm::vec3 walkTo)
{
//walk up to top point
float speed = 0.03f;
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



