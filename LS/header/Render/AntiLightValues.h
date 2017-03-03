#pragma once
#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/* Values for the 'Anti light grenade'
*/
struct AntiLightValues
{
	glm::vec2 _params;
	glm::vec3 _pos;

	AntiLightValues(){}

	AntiLightValues(glm::vec2 params, glm::vec3 pos) 
		: _params(params), _pos(pos)
	{
	}
};