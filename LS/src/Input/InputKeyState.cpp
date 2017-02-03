#include"Input/InputKeyState.h"

InputKeyState::InputKeyState(EventManager &manager, int key)
	: _listenKey(key), _keyAction(GLFW_PRESS), _active(false) {
	manager.listen(this, &InputKeyState::receive);
}


InputKeyState::~InputKeyState()
{
}

void InputKeyState::receive(const KeyboardEvent& event)
{
	if (event.getKey() == _listenKey && (event.getAction() != GLFW_REPEAT || _keyAction == GLFW_REPEAT)) {
		_active = event.getAction() == _keyAction;
	}
}