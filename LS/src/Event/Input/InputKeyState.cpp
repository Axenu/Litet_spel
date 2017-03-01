#include"Event/Input/InputKeyState.h"

InputKeyState::InputKeyState(EventManager *manager, int key)
	: _listenKey(key), _keyAction(GLFW_PRESS), _active(false)
{
	_eventManager = manager;
	_eventManager->listen(this, &InputKeyState::receive);
}


InputKeyState::~InputKeyState()
{
	_eventManager->unlisten(this, &InputKeyState::receive);
}

void InputKeyState::receive(const KeyboardEvent& event)
{
	if (event.getKey() == _listenKey && (event.getAction() != GLFW_REPEAT || _keyAction == GLFW_REPEAT)) {
		_active = event.getAction() == _keyAction;
	}
}
