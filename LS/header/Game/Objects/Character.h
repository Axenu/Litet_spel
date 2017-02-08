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
    void setCamera(Camera *camera);
    virtual void onUpdate(float dt);
    void onRender();

	void doYouWantToWin(const KeyboardEvent& event);
    void moveCharacter(const KeyboardEvent& event);
    void moveMouse(const MouseMoveEvent& event);
    void collectLoot(const CollectLootEvent& event);

	void setLevel(Grid *level);
	void setScene(Scene *scene);

	void move(float dT);

    Character(glm::vec3 pos, EventManager *manager);
    Character();
    virtual ~Character();
private:
	Grid *_currentLevel;
	Scene *_currentScene;
    EventManager *_eventManager;
	Camera* _camera;
    glm::vec2 _lastCursorPos;
	glm::ivec2 _gridSquare;
    glm::vec3 _direction;
    glm::vec3 _velocity;
    float _speed;
    float _isMoving;
    float _lootValue;
    bool _hasMoved = false;
    int _cursorMode = GLFW_CURSOR_DISABLED;
    bool _isAtExit = false;
};
