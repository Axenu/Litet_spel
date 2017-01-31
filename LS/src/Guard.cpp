#include "Guard.h"

Guard::~Guard()
{
}

Guard::Guard()
{

}
Guard::Guard(glm::vec3 Guarden):
GameObject()
{
	Guardsposition = Guarden;
	this->setPosition(Guardsposition);
}

void Guard::WalkingBetweenTwoPoints()
{
}



void Guard::Update(int dt)
{
	WalkingBetweenTwoPoints();
}
