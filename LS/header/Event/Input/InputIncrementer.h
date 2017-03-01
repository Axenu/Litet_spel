#pragma once
#include"InputManager.h"
#include "../EventManager.h"

/* Small input state object keeping track of a value range incremented with key presses.
*/
class InputIncrementer
{
public:
	/* If value loops during increment/decrement	*/
	bool _loop;
	int _value, _keyIncrement, _keyDecrement, _rangeMin, _rangeMax;
	EventManager *_eventManager;
	InputIncrementer(EventManager *manager, int incrementKey, int decrementKey, int min, int max);
	void receive(const KeyboardEvent& event);
	~InputIncrementer();
};
