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
		inSquare = false;
	}
}

void PossibleVictoryEvent::givepoints(float points)
{
	this->points = points;
}

bool PossibleVictoryEvent::victory(bool buttonpressed,bool onlyoncereset)
{
	if (onlyoncereset == true)
	{
		onlyonce = true;
	}
	if(onlyonce == true)
	{
		std::cout << "you won Congratulations Do you want to exit press G to exit?" << std::endl;
	}
	onlyonce = false;
	if (buttonpressed == true)
	{
		return true;
	}
	return false;
}
