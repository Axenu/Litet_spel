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

class ExitTriggerEvent : public Event
{
private:
	bool _didEnter;
public:
	ExitTriggerEvent(bool didEnter);
	~ExitTriggerEvent();
	bool didEnter() const;
};

class ChangeGameStateEvent : public Event
{
private:
	int _state;
public:
	ChangeGameStateEvent(int state);
	~ChangeGameStateEvent();

	int getState();

	static const int StoppedState = 0;
	static const int RunningState = 1;
	static const int PausedState = 2;
};

class GameStartedEvent : public Event
{
private:
	//pointer for player score
	//pointer for player stuff
public:
	GameStartedEvent();
	~GameStartedEvent();
};

class GameOverEvent : public Event
{
private:
	bool _hasWon;
public:
	GameOverEvent(bool hasWon);
	~GameOverEvent();
	bool hasWon() const;
};

class CanClimbEvent : public Event
{
private:
	bool _canClimb;
public:
	CanClimbEvent(bool _canClimb);
	~CanClimbEvent();
	bool canClimb() const;
};

class GuardAlertEvent : public Event
{
public:
	GuardAlertEvent(glm::vec3 pos, float det);
	~GuardAlertEvent();
	glm::vec3 _position;
	float _detection;
	unsigned int _id;
};
