#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class AABB
{
public:
    AABB(std::vector<glm::vec3> *position);
	AABB(const AABB &other);
	glm::vec3 getMin();
	glm::vec3 getMax();
	~AABB();
private:
	glm::vec3 _min;
	glm::vec3 _max;
};
