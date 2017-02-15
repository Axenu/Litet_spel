#include "Render/Mesh/Animation/Skeleton.h"



Skeleton::Skeleton(std::vector<Bone> &bones)
	: _bones(bones)
{
}


Skeleton::~Skeleton()
{
}

Animation* Skeleton::getAnimation(const std::string &name) {
	std::map<std::string, Animation>::iterator it = _animation.find(name);
	if (it != _animation.end()) //Check that value existed
		return &it->second;
	return nullptr;
}
std::vector<Bone> Skeleton::getBones() {
	return _bones;
}