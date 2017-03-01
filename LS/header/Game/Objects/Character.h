#pragma once

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "GameObject.h"
#include "Game/camera.h"
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "Game/Level/Grid.h"
#include "Game/Scene/Scene.h"
#include "AntiLightGrenade.h"
#include "StaticVars.h"

#define RotationSpeed 0.005f
#define SneakDiff 0.5f


class CollectLootEvent : public Event
{
public:
	CollectLootEvent(int value) : _value(value) {};

	int getValue() const { return _value; }

private:
	int _value;
};

enum CharState
{
	normal = 0,
	guardVision,
	climbing
};

class Character : public GameObject
{
public:
    virtual void onUpdate(float dt);
	virtual void init();
    void onRender();
	std::vector<GrenadeValues> getGrenadeData();
    void moveCharacter(const KeyboardEvent& event);
    void moveMouse(const MouseMoveEvent& event);

	void setLevel(Grid *level);
	void setScene(Scene *scene);
	void move(float dT);
	void climb(float dT);
	void tryClimb();
	void testClimb();
	bool guardVision();
	int getGrenadeID();
	int* getLootValuePointer();
	int* getGrenadeCountPointer();
	float* getGrenadeCooldownTimer();
	int amountOfGrenades();
	Character(glm::vec3 pos, EventManager *manager, std::vector<AntiLightGrenade *> grenade, float height);

    Character();
    virtual ~Character();

	glm::vec3 getEyePos();

	float calcLightOnPosition();

	float getLightAtPosition();
private:
	void normalKeyInput(const KeyboardEvent& event);
	void guardVisionKeyInput(const KeyboardEvent& event);
	//void charMoveMouse(const MouseMoveEvent& event);
	//void guardVisionMoveMouse(const MouseMoveEvent& event);
	void returnVision();
	CharState _state;
	float _height;
	int _grenadeID;
	Grid *_currentLevel;
	Scene *_currentScene;
    EventManager *_eventManager;
    glm::vec2 _lastCursorPos;
	GridSquare _gridSquare;
	glm::vec3 _velocity;
	/* Camera relative move dir. X: Right, Y: Forward */
	glm::vec2 _moveDir;
	glm::vec2 _camTilt;
	int _grenadeCount=0;
	float _timerForGrenade=6;
	std::vector<AntiLightGrenade*> _antiLightGrenade;
    float _speed;
    float _isMoving;
    int _lootValue;
    bool _hasMoved = false;
    int _cursorMode = GLFW_CURSOR_DISABLED;
	//Climbing variables
	bool _sneaking;
	bool _canClimb;
	glm::vec3 _animEndPos;
	float _animFirstPhaseTime;
	float _animSecondPhaseTime;
	float _animTime;
	float _animEndTime;
	float _heightDiff;
	int _lightGrenadeCount = 0;
	float LightGrenadeClock= 0;
	bool noMoreGrenadeCount;
	//Light variable
	float _lightAtPos;
};
