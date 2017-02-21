#pragma once

#include "gl/glInclude.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameObject.h"
#include "Game/Camera.h"

struct GrenadeValues {
	glm::vec4 _grenadePositionWhenLanded;
	float fading;
	float expanding;
};

class AntiLightGrenade : public GameObject
{
private:
float _fallspeed;
float _velocity;
glm::vec3 _direction;
bool QBeenPressed = true;
bool QbeenActivated = false;
bool TheBombHasBeenActivated = false;
glm::vec3 _movement;
GrenadeValues _grenadeValue;
public:
GrenadeValues getgrenadeData();
virtual	void update(float dt);
AntiLightGrenade(Model & m);
~AntiLightGrenade();
void ThrowTheLightgrenade(glm::vec3 CharacterPositions,glm::vec3 Direction);
};
