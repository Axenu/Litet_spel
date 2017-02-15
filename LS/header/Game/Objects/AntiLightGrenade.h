#pragma once

#include "gl/glInclude.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameObject.h"
class AntiLightGrenade : GameObject
{
private:
	float fallspeed;
	float velocity;
	bool QBeenPressed = false;
public:
virtual	void update(float dt);
AntiLightGrenade();
~AntiLightGrenade();
void ThrowTheLightgrenade(glm::vec2 CharacterPositions);
};