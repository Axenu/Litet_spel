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
#include "Game/Level/IObjectFactory.h"
#include <Sound/Irrklang.h>

#define RotationSpeed 0.005f
#define SneakDiff 0.5f

#define CLIMBINGNOISE 0.6f
#define WALKINGNOISE 0.4f
#define SNEAKINGMODIFIER 0.4f

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
    void moveCharacter(const KeyboardEvent& event);
    void moveMouse(const MouseMoveEvent& event);

	void setLevel(Grid *level);
	void setScene(Scene *scene);
	void move(float dT);
	void climb(float dT);
	void tryClimb();
	void testClimb();
	bool guardVision();
	int* getLootValuePointer();
	float* getScoreValuePointer();
	int* getGrenadeCountPointer();
	float* getGrenadeCooldownTimer();
	float getNoise();
	void pause();
	void resume();
	Character(glm::vec3 pos, EventManager *manager, int grenadeCount, float height);

    Character();
    virtual ~Character();

	glm::vec3 getEyePos();

	float calcLightAtPosition();

	float getLightAtPosition();
private:
	void normalKeyInput(const KeyboardEvent& event);
	void guardVisionKeyInput(const KeyboardEvent& event);
	bool charMovement(const KeyboardEvent& event);
	void detected(const GuardAlertEvent &event);
	void gVisionTimerUpdate(float dt);
	void calcNoise();
	void returnVision();
	CharState _state;
	float _height;

	Grid *_currentLevel;
	Scene *_currentScene;
    EventManager *_eventManager;
    glm::vec2 _lastCursorPos;
	GridSquare _gridSquare;
	glm::vec3 _velocity;
	/* Camera relative move dir. X: Right, Y: Forward */
	glm::vec2 _moveDir;
	glm::vec2 _camTilt;
    float _speed;
    float _isMoving;
    int _lootValue;
	float _score;
	float _detectionLevel;
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
	//AntiLightGranade  varables
	float _grenadeCooldown = 2;
	float _grenadeTimer = 6;
	int _lightGrenadeCount = 0;
	//Light variable
	float _lightAtPos;
	//GuardVision varable
	float _guardVisDuraTimer;
	//Noise variable
	float _movmentNoise;
};
