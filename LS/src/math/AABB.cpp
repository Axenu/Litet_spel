#include "math/AABB.h"

AABB::AABB() {

}
AABB::AABB(glm::vec3 pos, float cubeSize)
	: _min(pos - glm::vec3(cubeSize)), _max(pos + glm::vec3(cubeSize)) 
{
	_min.y = std::min(pos.y - cubeSize, 0.0f);
}
AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	: _min(min), _max(max) {
}
AABB::AABB(std::vector<glm::vec3> *position)
{
	if ((*position).size() > 0)
	{
		_min = (*position)[0];
		_max = (*position)[0];
	}
    for (std::vector<glm::vec3>::iterator it = position->begin(); it != position->end(); ++it) {
        _min.x = std::min(_min.x, it->x);
        _min.y = std::min(_min.y, it->y);
        _min.z = std::min(_min.z, it->z);
        _max.x = std::max(_max.x, it->x);
        _max.y = std::max(_max.y, it->y);
        _max.z = std::max(_max.z, it->z);
    }
    //Debug
     //std::cout<<glm::to_string(_min)<<std::endl;
     //std::cout<<glm::to_string(_max)<<std::endl;
}

AABB::AABB(std::vector<glm::vec4> *position)
{
	if ((*position).size() > 0)
	{
		_min = (*position)[0];
		_max = (*position)[0];
	}
	for (std::vector<glm::vec4>::iterator it = position->begin(); it != position->end(); ++it) {
		_min.x = std::min(_min.x, it->x);
		_min.y = std::min(_min.y, it->y);
		_min.z = std::min(_min.z, it->z);
		_max.x = std::max(_max.x, it->x);
		_max.y = std::max(_max.y, it->y);
		_max.z = std::max(_max.z, it->z);
	}
	//Debug
	//std::cout<<glm::to_string(_min)<<std::endl;
	//std::cout<<glm::to_string(_max)<<std::endl;
}

AABB::AABB(const AABB & other)
{
	_min = other._min;
	_max = other._max;
}
AABB AABB::transform(const glm::mat4 &matrix) const {
	std::vector<glm::vec4> points(8);
	points[0] = glm::vec4(_min.x, _min.y, _min.z, 1.0f);
	points[1] = glm::vec4(_min.x, _min.y, _max.z, 1.0f);
	points[2] = glm::vec4(_min.x, _max.y, _min.z, 1.0f);
	points[3] = glm::vec4(_min.x, _max.y, _max.z, 1.0f);
	points[4] = glm::vec4(_max.x, _min.y, _min.z, 1.0f);
	points[5] = glm::vec4(_max.x, _min.y, _max.z, 1.0f);
	points[6] = glm::vec4(_max.x, _max.y, _min.z, 1.0f);
	points[7] = glm::vec4(_max.x, _max.y, _max.z, 1.0f);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		points[i] = matrix * points[i];
	}
	return AABB(&points);
}


glm::vec3 AABB::getMin() const
{
	return _min;
}

glm::vec3 AABB::getMax() const
{
	return _max;
}

void AABB::setMin(glm::vec3 min)
{
	_min = min;
}

void AABB::setMax(glm::vec3 max)
{
	_max = max;
}

AABB::~AABB()
{

}
