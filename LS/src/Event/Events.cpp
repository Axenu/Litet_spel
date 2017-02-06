#include "Event/Events.h"

PossibleVictoryEvent::PossibleVictoryEvent()
{
	onlyonce = true;
	inSquare = false;
 points = 0;
}

PossibleVictoryEvent::~PossibleVictoryEvent()
{

}

void PossibleVictoryEvent::insquare(bool yesno)
{
	if (yesno)
	{
		inSquare = true;
	}
	else
	{
		false;
	}
}

void PossibleVictoryEvent::givepoints(float points)
{
	this->points = points;
}

bool PossibleVictoryEvent::victory(bool buttonpressed)
{

	if(onlyonce == true)
	{
		std::cout << "you won Congratulations" << std::endl;
	}
	onlyonce = false;
	if (buttonpressed == true)
	{
		return true;
	}
	return false;
}
