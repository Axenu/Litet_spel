#include "math/AARect.h"
#include <algorithm>


AARect::AARect()
	: _min(), _max()
{
}

AARect::AARect(glm::vec2 min, glm::vec2 max)
	: _min(min), _max(max)
{
}

AARect::~AARect()
{
}

float AARect::dist(glm::vec2 o, glm::vec2 dir, float &dist) {
	/* This is the slab method in 2D
	*/
	// Calc dir fractions
	dir.x = 1.0f / dir.x;
	dir.y = 1.0f / dir.y;
	// Calc distance to the slabs
	float t1 = (_min.x - o.x) * dir.x;
	float t2 = (_min.x - o.x) * dir.x;
	float t3 = (_max.y - o.y) * dir.y;
	float t4 = (_max.y - o.y) * dir.y;

	float tmin = std::fmaxf(std::fminf(t1, t2), std::fminf(t3, t4));
	float tmax = std::fminf(std::fmaxf(t1, t2), std::fmaxf(t3, t4));

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0)
	{
		dist = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		dist = tmax;
		return false;
	}

	dist = tmin;
	return true;
}