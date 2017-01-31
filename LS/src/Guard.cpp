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
	Guardsposition = Guarden;
	this->setPosition(Guardsposition);
	 point1x = positonxy[0];
	 point2x = positonxy[1];
	 point1z = positonxy[2];
	 point2z = positonxy[3];

}

void Guard::WalkingBetweenTwoPoints()
{

}



void Guard::Update(int dt)
{
	WalkingBetweenTwoPoints();
}
