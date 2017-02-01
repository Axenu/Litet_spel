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

void Guard::WalkingBetweenTwoPoints()
{

}




void Guard::onUpdate(float dt)
{

//	WalkingBetweenTwoPoints();
}

Guard::Guard(Model &m, Grid *gridet):
	GameObject(m)
{
	gridet->Createxandypoint12(glm::vec3(gridet->getData(guard).xz.x, 0, gridet->getData(guard).xz.y));
	point1x = gridet->getxandypoint12(0);
	point2x = gridet->getxandypoint12(1);
	point1z = gridet->getxandypoint12(2);
	point2z = gridet->getxandypoint12(3);
	this->setPosition(glm::vec3(gridet->getData(guard).xz.x, .6, gridet->getData(guard).xz.y));


}

void Guard::setPositionfromMap(glm::vec3 Guarden)
{
	this->setPosition(Guarden);

}



