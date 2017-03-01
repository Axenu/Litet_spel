#include "Event/Input/InputIncrementer.h"


InputIncrementer::InputIncrementer(EventManager *manager, int incrementKey, int decrementKey, int min, int max)
	: _value(min), _keyIncrement(incrementKey), _keyDecrement(decrementKey), _rangeMin(min), _rangeMax(max)
{
	_eventManager = manager;
	_eventManager->listen(this, &InputIncrementer::receive);
}


InputIncrementer::~InputIncrementer()
{
	_eventManager->unlisten(this, &InputIncrementer::receive);
}

void InputIncrementer::receive(const KeyboardEvent& event)
{
	if (event.getAction() == GLFW_PRESS) {
		if (event.getKey() == _keyDecrement) {
			_value--;
			if (_value < _rangeMin)
				_value = _loop ? _rangeMax : _rangeMin;
		}
		else if (event.getKey() == _keyIncrement) {
			_value++;
			if (_value > _rangeMax)
				_value = _loop ? _rangeMin : _rangeMax;
		}
	}
}
