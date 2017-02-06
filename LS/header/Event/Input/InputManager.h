#pragma once

#include "gl/glinclude.h"
#include "Event/EventManager.h"
#define GLM_FORCE_RADIANS
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

    int getX() const {return (int)_x;}
    int getY() const {return (int)_y;}
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

class cursorModeChangeEvent : public Event
{
public:
    cursorModeChangeEvent(int state) : _state(state) {};

    int getState() const {return _state;}

private:
    int _state;
};

class QuitGameEvent : public Event
{
public:
    QuitGameEvent() {};
};

class InputManager
{
public:
    InputManager(GLFWwindow *window, EventManager* manager);
    ~InputManager();

    void switchCursorMode(const cursorModeChangeEvent& event);
    void quitGame(const QuitGameEvent& event);
    // int getCursorMode();

    EventManager* getManager();
private:
    EventManager* _manager;
    GLFWwindow* _window;
    int _cursorMode;
};
