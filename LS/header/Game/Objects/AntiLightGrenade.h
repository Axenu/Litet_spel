#pragma once

#include "gl/glInclude.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "GameObject.h"
#include "Game/Camera.h"
class AntiLightGrenade : public GameObject
{
private:
float _fallspeed;
float _velocity;
glm::vec3 _direction;
glm::mat4 viewMatrix;
bool QBeenPressed = true;
bool QbeenActivated = false;
glm::vec3 _movement;
glm::vec4 _grenadePositionWhenLanded;
glm::mat4 projMatrix;
public:
glm::vec4 getgrenadePositionWhenlanded();
virtual	void update(float dt);
AntiLightGrenade(Model & m);
~AntiLightGrenade();
void ThrowTheLightgrenade(glm::vec3 CharacterPositions,glm::vec3 Direction);
};
