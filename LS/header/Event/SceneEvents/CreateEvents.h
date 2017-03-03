#pragma once
#define GLM_FORCE_RADIANS
#include "Event/EventManager.h"
#include <glm/vec3.hpp>

class CreateLightGrenade : public Event
{
public:
	CreateLightGrenade(std::string model, glm::vec3 pos, glm::vec3 dir) : _model(model), _pos(pos), _dir(dir) {};
	std::string _model;
	const glm::vec3 _pos, _dir;
};