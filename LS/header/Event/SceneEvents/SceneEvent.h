#pragma once
#include "Event/EventManager.h"
#include "Game/Objects/GameObject.h"

/* Event for removing an element from the scene
*/
class RemoveSceneObject : public Event
{
public:
	RemoveSceneObject(GameObject* obj) : _obj(obj) {};
	GameObject* _obj;
};