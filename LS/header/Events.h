#pragma once

#include "EventManager.h"
#include "GridDataStructure.h"
class PossibleVictoryEvent : public Event
{
private:
	bool inSquare;
	float points;
	bool onlyonce;

public:
	PossibleVictoryEvent();
	~PossibleVictoryEvent();
void insquare(bool yesno);
void givepoints(float points);
void victory();
};
