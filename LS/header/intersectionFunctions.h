#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <gl/GLFunctions.h>
#include "AABB.h"


bool PointInTriangle(glm::vec4, glm::vec4, glm::vec4, glm::vec4);

bool TriangleIntersection(glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3, glm::vec3 origin, glm::vec3 dir);

bool TriangleIntersection(glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3, glm::vec3 origin, glm::vec3 dir, glm::vec3& point);

bool AABBIntersection(AABB &aabb, glm::vec3, glm::vec3);

