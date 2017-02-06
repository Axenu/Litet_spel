#pragma once
#include"InputManager.h"
#include "../EventManager.h"

/* Input state object keeping track of a key press state
*/
class InputKeyState
{
private:
	int _keyAction;
	int _listenKey;
public:
	bool _active;
	InputKeyState(EventManager &manager, int key);
	void receive(const KeyboardEvent& event);
	~InputKeyState();
};
