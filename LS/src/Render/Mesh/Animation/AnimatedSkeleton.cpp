#include "Render/Mesh/Animation/AnimatedSkeleton.h"



AnimatedSkeleton::AnimatedSkeleton(Skeleton& ref)
	: _ref(ref)
{
}


AnimatedSkeleton::~AnimatedSkeleton()
{
}

void AnimatedSkeleton::update(float dT) {
	if (_animation) {
		_elapAnimTime += dT;
		//Check if animation is complete, currently it will only loop
		if (_elapAnimTime > _animation->_duration)
			loopRefit(); //Loop

		//Update keys if necessary
		for (int i = 0; i < boneCount(); i++) {
			if (_toKeys[i]._time < _elapAnimTime)
				nextKey(i);
		}

		//Calculate skin transforms.
		const glm::mat4 &root = _root->getModelMatrix();
		_pose[0] = _fromKeys[0].lerpTo(_toKeys[0], _elapAnimTime);
		for (unsigned int i = 1; i < boneCount(); i++) {
			const Bone &bone = _ref.getBones()[i];
			_pose[i] = _fromKeys[i].lerpTo(_toKeys[i], _elapAnimTime) * _pose[bone._parentIndex];
			_skinTransform[i] = root * _pose[i] * bone._invBindPose;
		}
	}
}

/* Recalculates the end time when animation is looped */
void AnimatedSkeleton::loopRefit(){
	_elapAnimTime = fmod(_elapAnimTime, _animation->_duration); //loop
	for (unsigned int i = 0; i < boneCount(); i++)
		loopKey(i);
}
void AnimatedSkeleton::nextKey(int bone) {
	int key = _animation->goToFrame(bone, _elapAnimTime);
	KeyFrame old = _fromKeys[bone];
	_fromKeys[bone] = _toKeys[bone]; //Animate from last key to next
	if (key != -1)
		_toKeys[bone] = _animation->_keys[bone][key];
	else
		_toKeys[bone] = KeyFrame(_animation->_duration, old._transform, false);
}
void AnimatedSkeleton::loopKey(int bone) {
	if (_toKeys[bone]._custom)
		_toKeys[bone]._time -= _animation->_duration;
	else
		nextKey(bone);
}


const std::vector<glm::mat4>& AnimatedSkeleton::getSkin() const {
	return _skinTransform;
}
unsigned int AnimatedSkeleton::boneCount() const {
	return _pose.size();
}