#pragma once

#include <iostream>
#include"GameObject.h"
#include "camera.h"
#include "InputManager.h"
#include "EventManager.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "GridDataStructure.h"

#define RotationSpeed 0.005f

class CollectLootEvent : public Event
{
public:
    CollectLootEvent(float value) : _value(value) {};

    float getValue() const {return _value;}

private:
    float _value;
};

class Character : public GameObject
{
public:
    void setCamera(Camera *camera);

    virtual void update(float dt);
    void onRender();

    void moveCharacter(const KeyboardEvent& event);
    void moveMouse(const MouseMoveEvent& event);
    void collectLoot(const CollectLootEvent& event);

	void setLevel(Grid *level);

    Character(glm::vec3 pos, EventManager *manager);
    Character();
    ~Character();
private:
	Grid *_currentLevel;
    EventManager *_eventManager;
    Camera* _camera;
    glm::vec2 _lastCursorPos;
    glm::vec3 _velocity;
    float _lootValue;
    bool _hasMoved = false;
    int _cursorMode = GLFW_CURSOR_NORMAL;
};
