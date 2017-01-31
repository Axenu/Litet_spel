#include "AABB.h"

AABB::AABB(std::vector<glm::vec3> *position)
{
    for (std::vector<glm::vec3>::iterator it = position->begin(); it != position->end(); ++it) {
        _min.x = fmin(_min.x, it->x);
        _min.y = fmin(_min.y, it->y);
        _min.z = fmin(_min.z, it->z);
        _max.x = fmax(_max.x, it->x);
        _max.y = fmax(_max.y, it->y);
        _max.z = fmax(_max.z, it->z);
    }
    //Debug
    // std::cout<<glm::to_string(_min)<<std::endl;
    // std::cout<<glm::to_string(_max)<<std::endl;
}

AABB::~AABB()
{

}
