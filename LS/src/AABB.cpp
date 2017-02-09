#include "AABB.h"

AABB::AABB() {

}
AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	: _min(min), _max(max) {
}
AABB::AABB(std::vector<glm::vec3> *position)
{
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

AABB::AABB(const AABB & other)
{
	_min = other._min;
	_max = other._max;
}
AABB AABB::transform(const glm::mat4 &matrix) const {
	return AABB(matrix * glm::vec4(_min, 1.0f), matrix * glm::vec4(_max, 1.0f));
}


glm::vec3 AABB::getMin() const
{
	return _min;
}

glm::vec3 AABB::getMax() const
{
	return _max;
}

AABB::~AABB()
{

}
