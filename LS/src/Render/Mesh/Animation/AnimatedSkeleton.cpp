#include "Render/Mesh/Animation/AnimatedSkeleton.h"



AnimatedSkeleton::AnimatedSkeleton(Skeleton& ref, Node& root)
	: _ref(ref), _root(root), _channel(ref.getNumBones()), _pose(ref.getNumBones()), _skinTransform(ref.getNumBones())
{
	//Each channel needs to be initiated so it can generate it's own frames.
	for (unsigned int i = 0; i < ref.getNumBones(); i++)
		_channel[i].init(ref.getBone(i)._bindPose);
	updateSkeleton();
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
		updateSkeleton();
	}
}
void AnimatedSkeleton::updateSkeleton() 
{
	//Update skeleton
	_pose[0] = _channel[0].lerp(_elapAnimTime);
	_skinTransform[0] = _pose[0] * _ref.getBone(0)._invBindPose;
	for (unsigned int i = 1; i < boneCount(); i++)
	{
		const Bone& bone = _ref.getBone(i);
		_pose[i] = _pose[bone._parentIndex] * _channel[i].lerp(_elapAnimTime);
		_skinTransform[i] =  _pose[i] * bone._invBindPose;
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