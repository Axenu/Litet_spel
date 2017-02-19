#include "Render/Mesh/Animation/AnimatedSkeleton.h"



AnimatedSkeleton::AnimatedSkeleton(Skeleton& ref)
	: _ref(ref)
{
}


AnimatedSkeleton::~AnimatedSkeleton()
{
}

void AnimatedSkeleton::update(float dT) {
	if (_animation) //If animation running
	{
		_elapAnimTime += dT;
		//Check if animation is complete, currently it will only loop
		if (_elapAnimTime > _animation->_duration)
			loopRefit(); //Loop
		
		//Calculate skin transforms.
		const glm::mat4 &root = _root->getModelMatrix();
		_pose[0] = _channel[0].lerp(_elapAnimTime);
		for (unsigned int i = 1; i < boneCount(); i++) 
		{
			const Bone &bone = _ref.getBones()[i];
			_pose[i] = _channel[i].lerp(_elapAnimTime) * _pose[bone._parentIndex];
			_skinTransform[i] = root * _pose[i] * bone._invBindPose;
		}
	}
}

/* Recalculates the end time when animation is looped */
void AnimatedSkeleton::loopRefit(){
	_elapAnimTime = fmod(_elapAnimTime, _animation->_duration); //loop
	for (unsigned int i = 0; i < boneCount(); i++)
		_channel[i].loop(_elapAnimTime, _animation->_duration);
}


const std::vector<glm::mat4>& AnimatedSkeleton::getSkin() const {
	return _skinTransform;
}
unsigned int AnimatedSkeleton::boneCount() const {
	return _pose.size();
}