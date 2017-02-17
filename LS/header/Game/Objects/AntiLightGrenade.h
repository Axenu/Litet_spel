#pragma once

#include "gl/glInclude.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameObject.h"
class AntiLightGrenade : public GameObject
{
private:

float _fallspeed;
float _velocity;
glm::vec3 _direction;
bool QBeenPressed = true;
bool QbeenActivated = false;
glm::vec3 movement;
public:

virtual	void update(float dt);
AntiLightGrenade(Model & m);
~AntiLightGrenade();
void ThrowTheLightgrenade(glm::vec3 CharacterPositions,glm::vec3 Direction);
};