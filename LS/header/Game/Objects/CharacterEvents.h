#pragma once
#include "Event/Events.h"
#include "Character.h"
#include "GridDataStructure.h"

/* Event triggered when character moves to the next square
*/
class CharacterSquareEvent : public Event
{
public:
	CharacterSquareEvent(Character *ref, glm::ivec2 square, gridType type) : _ref(ref), _square(square), _type(type) {};

	Character *const _ref;
	const glm::ivec2 _square;
	const gridType _type;

};