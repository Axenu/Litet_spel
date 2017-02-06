#pragma once

#include "EventManager.h"
#include "gl/glInclude.h"
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
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
bool victory(bool buttonpressed,bool);
};
