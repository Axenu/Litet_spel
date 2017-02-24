#include "Event/Events.h"

ExitTriggerEvent::ExitTriggerEvent(bool didEnter)
{
	_didEnter = didEnter;
}
ExitTriggerEvent::~ExitTriggerEvent()
{

}
bool ExitTriggerEvent::didEnter() const
{
	return _didEnter;
}
ChangeGameStateEvent::ChangeGameStateEvent(int state) : _state(state)
{

}
ChangeGameStateEvent::~ChangeGameStateEvent()
{

}
int ChangeGameStateEvent::getState()
{
	return _state;
}
GameStartedEvent::GameStartedEvent()
{

}
GameStartedEvent::~GameStartedEvent()
{

}
GameOverEvent::GameOverEvent(bool hasWon) : _hasWon(hasWon)
{
}
GameOverEvent::~GameOverEvent()
{

}
bool GameOverEvent::hasWon() const
{
	return _hasWon;
}


CanClimbEvent::CanClimbEvent(bool canClimb)
{
	_canClimb = canClimb;
}

CanClimbEvent::~CanClimbEvent()
{
}

bool CanClimbEvent::canClimb() const
{
	return _canClimb;
}


CanClimbEvent::CanClimbEvent(bool canClimb)
{
	_canClimb = canClimb;
}

CanClimbEvent::~CanClimbEvent()
{
}

bool CanClimbEvent::canClimb() const
{
	return _canClimb;
}


CanClimbEvent::CanClimbEvent(bool canClimb)
{
	_canClimb = canClimb;
}

CanClimbEvent::~CanClimbEvent()
{
}

bool CanClimbEvent::canClimb() const
{
	return _canClimb;
}

AlmostDetectedEvent::AlmostDetectedEvent(bool almostDetected)
{
	_almostDetected = almostDetected;
}

AlmostDetectedEvent::~AlmostDetectedEvent()
{

}

bool AlmostDetectedEvent::almostDetected() const
{
	return _almostDetected;
}
