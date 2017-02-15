#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <gl/GLFunctions.h>
#include "AABB.h"


struct Plane
{
	glm::vec3 normal;
	float distance;
};

enum PlaneResult
{
	Outside = 0,
	Inside,
	Intersecting
};

bool PointInTriangle(glm::vec4, glm::vec4, glm::vec4, glm::vec4);

bool TriangleIntersection(glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3, glm::vec3 origin, glm::vec3 dir);

bool TriangleIntersection(glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3, glm::vec3 origin, glm::vec3 dir, glm::vec3& point);

bool AABBIntersection(AABB &aabb, glm::vec3, glm::vec3);

PlaneResult BBPlaneTest(glm::vec3 BBMin, glm::vec3 BBmax, glm::vec3 pNormal, float pDistance);

PlaneResult BBPlaneTest(glm::vec3 BBMin, glm::vec3 BBmax, const Plane &plane);

PlaneResult BBPlaneTest(const AABB &aabb, const Plane &plane);

