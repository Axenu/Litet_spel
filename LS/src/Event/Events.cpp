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
