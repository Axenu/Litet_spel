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

void PossibleVictoryEvent::victory()
{

	if(onlyonce == true)
	{
	std::cout << "you won Congratulations" << std::endl;
	}
	onlyonce = false;
}
