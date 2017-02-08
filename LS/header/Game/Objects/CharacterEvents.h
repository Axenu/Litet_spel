#pragma once
#include "Event/Events.h"
#include "Character.h"
#include "Game/Level/GridSquare.h"

/* Event triggered when character moves to the next square
*/
class CharacterSquareEvent : public Event
{
public:
	CharacterSquareEvent(Character *ref, GridSquare square) : _ref(ref), _square(square) {};
	~CharacterSquareEvent() {}

	Character *const _ref;
	const GridSquare _square;

};
