#pragma once

#include "gl/glinclude.h"
#include "EventManager.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <map>

class KeyboardEvent : public Event
{
public:
    KeyboardEvent(int keyID, int action) : _keyID(keyID), _action(action) {};

    int getKey() const {return _keyID;}
    int getAction() const {return _action;}

private:
    int _keyID;
    int _action;
};

class MouseMoveEvent : public Event
{
public:
    MouseMoveEvent(double x, double y) : _x(x), _y(y) {};

    int getX() const {return _x;}
    int getY() const {return _y;}
    glm::vec2 getPos() const {return glm::vec2(_x, _y);}

private:
    double _x;
    double _y;
};

class MouseClickEvent : public Event
{
public:
    MouseClickEvent(int keyID, int action) : _keyID(keyID), _action(action) {};

    int getKey() const {return _keyID;}
    int getAction() const {return _action;}

private:
    int _keyID;
    int _action;
};

class InputManager
{
public:
    InputManager(GLFWwindow *window, EventManager* manager);
    ~InputManager();

    EventManager* getManager();
private:
    EventManager* _manager;
};
