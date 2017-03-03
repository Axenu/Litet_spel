#pragma once

#include "gl/glInclude.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameObject.h"
#include "Game/Camera.h"
#include "Game/Level/Grid.h"
#include "Event/EventManager.h"
	const static float expandSize = 3.f;
	const static float expandDur = 1.f;
	const static float fadeDur = 3.f;

struct GrenadeValues {
	float fading;
	float expanding;
};

class AntiLightGrenade : public GameObject
{
private:

	float _fallspeed;
	float _velocity;
	glm::vec3 _direction;
	float timer = 0;
	glm::vec3 _movement;
	GrenadeValues _grenadeValue;
	Grid *_currentLevel;
	EventManager &_eventManager;
	bool _landed;
public:
	void setLevel(Grid *level);
	GrenadeValues getgrenadeData();
	virtual	void update(float dt);
	AntiLightGrenade(EventManager &_eventManager, Model & m, glm::vec3 CharacterPositions, glm::vec3 Direction);
	~AntiLightGrenade();
	void ThrowTheLightgrenade(glm::vec3 CharacterPositions, glm::vec3 Direction);

	/* Add the object to the frame
	*/
	virtual void addToFrame(DrawFrame &dF);
};
