#include "Render/Mesh/Animation/Skeleton.h"



Skeleton::Skeleton(std::vector<Bone> &bones)
	: _bones(bones)
{
}


Skeleton::~Skeleton()
{
}

Animation* Skeleton::getAnimation(const std::string &name) {
	std::map<std::string, std::shared_ptr<Animation>>::iterator it = _animation.find(name);
	if (it != _animation.end()) //Check that value existed
		return it->second.get();
	return nullptr;
}
const Bone& Skeleton::getBone(unsigned int i) const {
	return _bones[i];
}
const std::vector<Bone>& Skeleton::getBones() const {
	return _bones;
}
unsigned int Skeleton::getNumBones() const {
	return _bones.size();
}
/* Add an animation to the skeleton, if animation with the same name exists it will be removed. */
void Skeleton::addAnimation(std::shared_ptr<Animation> &anim) {
	_animation[anim->_name] = anim;
}