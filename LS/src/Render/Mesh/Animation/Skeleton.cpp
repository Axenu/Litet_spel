#include "Render/Mesh/Animation/Skeleton.h"



Skeleton::Skeleton(std::vector<Bone> &bones)
	: _bones(bones)
{
}


Skeleton::~Skeleton()
{
}


void Skeleton::update(float dT) {
	_pose[0] = _animation[0];
	for (unsigned int i = 1; i < _bones.size(); i++) {
		_pose[i] = _animation[i] * _pose[_bones[i]._parentIndex];
		_skinTransform[i] = _root->getModelMatrix() * _pose[i] * _bones[i]._invBindPose;
	}
}
const std::vector<glm::mat4>& Skeleton::getSkin() const {
	return _skinTransform;
}