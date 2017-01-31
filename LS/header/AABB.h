#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

class AABB
{
private:
    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _min;
    glm::vec3 _max;

public:
    AABB(std::vector<glm::vec3> *position);
};
