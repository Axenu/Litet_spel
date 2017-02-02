#include "Picking.h"

bool PointInTriangle(glm::vec3 triV1, glm::vec3 triV2, glm::vec3 triV3, glm::vec3 pointInTriangle)
{
	glm::vec3 cp1 = glm::cross((glm::vec3(triV3.x, triV3.y, triV3.z) - glm::vec3(triV2.x, triV2.y, triV2.z)), (glm::vec3(pointInTriangle.x, pointInTriangle.y, pointInTriangle.z) - glm::vec3(triV2.x, triV2.y, triV2.z)));
	glm::vec3 cp2 = glm::cross((glm::vec3(triV3.x, triV3.y, triV3.z) - glm::vec3(triV2.x, triV2.y, triV2.z)), (glm::vec3(triV1.x, triV1.y, triV1.z) - glm::vec3(triV2.x, triV2.y, triV2.z)));

	if (glm::dot(cp1, cp2) >= 0)
	{
		cp1 = glm::cross((glm::vec3(triV3.x, triV3.y, triV3.z) - glm::vec3(triV1.x, triV1.y, triV1.z)), (glm::vec3(pointInTriangle.x, pointInTriangle.y, pointInTriangle.z) - glm::vec3(triV1.x, triV1.y, triV1.z)));
		cp2 = glm::cross((glm::vec3(triV3.x, triV3.y, triV3.z) - glm::vec3(triV1.x, triV1.y, triV1.z)), (glm::vec3(triV2.x, triV2.y, triV2.z) - glm::vec3(triV1.x, triV1.y, triV1.z)));

		if (glm::dot(cp1, cp2) >= 0)
		{
			cp1 = glm::cross((glm::vec3(triV2.x, triV2.y, triV2.z) - glm::vec3(triV1.x, triV1.y, triV1.z)), (glm::vec3(pointInTriangle.x, pointInTriangle.y, pointInTriangle.z) - glm::vec3(triV1.x, triV1.y, triV1.z)));
			cp2 = glm::cross((glm::vec3(triV2.x, triV2.y, triV2.z) - glm::vec3(triV1.x, triV1.y, triV1.z)), (glm::vec3(triV3.x, triV3.y, triV3.z) - glm::vec3(triV1.x, triV1.y, triV1.z)));

			if (glm::dot(cp1, cp2) >= 0)
			{
				return true;
			}

			return false;
		}

		return false;
	}

	return false;
}

bool TriangleIntersection(glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3, glm::vec3 origin, glm::vec3 dir)
{


	glm::vec3 u(0.0f, 0.0f, 0.0f);
	glm::vec3 v(0.0f, 0.0f, 0.0f);
	glm::vec3 normal(0.0f, 0.0f, 0.0f);

	u = tri2 - tri1;
	v = tri3 - tri1;

	normal = glm::cross(u, v);

	normal = glm::normalize(normal);

	float tri1A = normal.x;
	float tri1B = normal.y;
	float tri1C = normal.z;
	float tri1D = (-(tri1A * tri1.x) - (tri1B * tri1.y) - (tri1C * tri1.z));

	float ep1, ep2, t = 0.0f;
	float planeIntersectX, planeIntersectY, planeIntersectZ = 0.0f;

	ep1 = (origin.x * tri1A) + (origin.y * tri1B) + (origin.z * tri1C);
	ep2 = (dir.x * tri1A) + (dir.y * tri1B) + (dir.z * tri1C);

	if (ep2 > 0.000001f || -0.000001f > ep2)
	{
		t = -(ep1 + tri1D) / ep2;
	}
	else
	{
		return false;
	}

	if (t > 0.0f)
	{
		planeIntersectX = origin.x + dir.x * t;
		planeIntersectY = origin.y + dir.y * t;
		planeIntersectZ = origin.z + dir.z * t;

		glm::vec3 pointInPlane(planeIntersectX, planeIntersectY, planeIntersectZ);

		if (PointInTriangle(tri1, tri2, tri3, pointInPlane))
		{
			return true;
		}
	}

	return false;
}

bool AABBIntersection(AABB &aabb, glm::vec3 dir, glm::vec3 origin)
{
	glm::vec3 min = aabb.getMin();
	glm::vec3 max = aabb.getMax();

	float tx1 = (min.x - origin.x) / dir.x;
	float tx2 = (max.x - origin.x) / dir.x;

	float tmin = glm::min(tx1, tx2);
	float tmax = glm::max(tx1, tx2);

	if (tmax < 0)
	{
		return false;
	}

	float ty1 = (min.y - origin.y) / dir.y;
	float ty2 = (max.y - origin.y) / dir.y;

	float tymin = glm::min(ty1, ty2);
	float tymax = glm::max(ty1, ty2);

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	tmin = glm::max(tmin, tymin);
	tmax = glm::min(tmax, tymax);
	if (tmax < 0)
	{
		return false;
	}

	float tz1 = (min.z - origin.z) / dir.z;
	float tz2 = (max.z - origin.z) / dir.z;

	float tzmin = glm::min(tz1, tz2);
	float tzmax = glm::max(tz1, tz2);

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	tmin = glm::max(tmin, tzmin);
	tmax = glm::min(tmax, tzmax);

	if (tmax < 0)
	{
		return false;
	}

	if (tmax < tmin)
	{
		return false;
	}

	return true;
}
