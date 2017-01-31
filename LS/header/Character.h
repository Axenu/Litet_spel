#pragma once

#include <iostream>
#include "node.h"
#include "camera.h"
#include "InputManager.h"
#include "EventManager.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define RotationSpeed 0.005f

class CollectLootEvent : public Event
{
public:
    CollectLootEvent(float value) : _value(value) {};

    float getValue() const {return _value;}

private:
    float _value;
};

class Character : public Node
{
public:
    void setCamera(Camera *camera);

    void onUpdate(float dt);
    void onRender();

    void moveCharacter(const KeyboardEvent* event);
    void moveMouse(const MouseMoveEvent* event);
    void collectLoot(const CollectLootEvent* event);

    Character(EventManager *manager);
    Character();
    ~Character();
private:
    EventManager *_eventManager;
    Camera* _camera;
    glm::vec2 _lastCursorPos;
    glm::vec3 _velocity;
    float _lootValue;
    bool _hasMoved = false;
    int _cursorMode = GLFW_CURSOR_NORMAL;
};
