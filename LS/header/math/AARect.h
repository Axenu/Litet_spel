#pragma once

#include <glm/vec2.hpp>

class AARect
{

public:

	glm::vec2 _min, _max;
	AARect();
	AARect(glm::vec2 min, glm::vec2 max);
	~AARect();

	float dist(glm::vec2 o, glm::vec2 dir, float &dist);

};

