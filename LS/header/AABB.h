#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <algorithm>

class AABB
{
public:
	AABB();
	AABB(const glm::vec3& min, const glm::vec3& max);
    AABB(std::vector<glm::vec3> *position);
	AABB(std::vector<glm::vec4> *position);
	AABB(glm::vec3 position);
	AABB(const AABB &other);
	glm::vec3 getMin() const;
	glm::vec3 getMax() const;
	void setMin(glm::vec3 min);
	void setMax(glm::vec3 max);
	~AABB();

	AABB transform(const glm::mat4 &matrix) const;
private:
	glm::vec3 _min;
	glm::vec3 _max;
};
