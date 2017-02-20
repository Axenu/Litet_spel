#include "Render/Mesh/Animation/AnimatedSkeleton.h"



AnimatedSkeleton::AnimatedSkeleton(Skeleton& ref, Node& root)
	: _ref(ref), _root(root), _channel(ref.getNumBones()), _pose(ref.getNumBones()), _skinTransform(ref.getNumBones())
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
		const glm::mat4 &root = _root.getModelMatrix();
		const Bone &bone_0 = _ref.getBones()[0];
		//_pose[0] = _channel[0].lerp(_elapAnimTime);
		_pose[0] = bone_0._bindPose;
		_skinTransform[0] = root * _pose[0] * bone_0._invBindPose;
		for (unsigned int i = 1; i < boneCount(); i++) 
		{
			const Bone &bone = _ref.getBones()[i];
			_pose[i] = bone._bindPose * _pose[bone._parentIndex];
			//_pose[i] = _channel[i].lerp(_elapAnimTime) * _pose[bone._parentIndex];
			_skinTransform[i] = root * _pose[i] * bone._invBindPose;
		}
	}
}

bool AnimatedSkeleton::setAnim(const std::string& name) {
	//Change animation, it may fail
	_animation = _ref.getAnimation(name);
	for (unsigned int i = 0; i < _channel.size(); i++)
		_channel[i].newAnimation(_elapAnimTime, &_animation->operator[](i), _animation->_duration);
	_elapAnimTime = 0.f;
	return _animation != nullptr;
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