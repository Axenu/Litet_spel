#pragma once

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Game/camera.h"
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "GridDataStructure.h"
#include "Game/Scene/Scene.h"

#define RotationSpeed 0.005f


class CollectLootEvent : public Event
{
public:
	CollectLootEvent(int value) : _value(value) {};

	int getValue() const { return _value; }

private:
	int _value;
};
class Character : public GameObject
{
public:
    void init();
    virtual void onUpdate(float dt);
    void onRender();

    void moveCharacter(const KeyboardEvent& event);
    void moveMouse(const MouseMoveEvent& event);

	void setLevel(Grid *level);
	void setScene(Scene *scene);

	void move(float dT);
	int* getLootValuePointer();

    Character(glm::vec3 pos, EventManager *manager);
    Character();
    virtual ~Character();
private:
	Grid *_currentLevel;
	Scene *_currentScene;
    EventManager *_eventManager;
    glm::vec2 _lastCursorPos;
	GridSquare _gridSquare;
    glm::vec3 _direction;
    glm::vec3 _velocity;
    float _speed;
    float _isMoving;
    int _lootValue;
    bool _hasMoved = false;
    int _cursorMode = GLFW_CURSOR_DISABLED;
};
