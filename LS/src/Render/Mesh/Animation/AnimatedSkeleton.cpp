#include "Render/Mesh/Animation/AnimatedSkeleton.h"
#include "Render/Mesh/Animation/BoneNode.h"


AnimatedSkeleton::AnimatedSkeleton(Skeleton& ref, Node& root)
	: _ref(ref), _root(root), _channel(ref.getNumBones()), _pose(ref.getNumBones()), _skinTransform(ref.getNumBones()), _boneNodes()
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
		if (_elapAnimTime > _animDuration)
		{
			//Check what state 
			switch (_endState)
			{
			case AnimatedSkeleton::Loop:
				loopRefit();
				break;
			//Stop animation
			case AnimatedSkeleton::Once:
				/* Special case for (pose) animations of 0 or < dT duration. Updates the skeleton.
				 * Check could be performed when setting animation but this includes dT verification.
				*/
				if (_animDuration < dT)
				{
					_elapAnimTime = _animDuration * 0.5f;
					updateSkeleton();
				}
			default:
				stopAnimation();
				return;
			}
		}
		//Update
		updateSkeleton();
	}
	//Update bone nodes
	for (unsigned int i = 0; i < _boneNodes.size(); i++)
		_boneNodes[i]->update(dT);
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

bool AnimatedSkeleton::setAnim(const std::string& name, PlayType runType) {
	//Change animation, it can be null
	_animation = _ref.getAnimation(name);
	if (_animation)
	{
		_animDuration = _animation->_duration;
		for (unsigned int i = 0; i < _channel.size(); i++)
			_channel[i].newAnimation(_elapAnimTime, &_animation->operator[](i), 0.f);
		_elapAnimTime = 0.f;
		_endState = runType;
		return true;
	}
	//No animation found, set current to null.
	stopAnimation();
	return false;
}

/* Pose the skeleton at the specific point of animation. Blends into the pose over the animation time specified. */
bool AnimatedSkeleton::setAnimPose(const std::string& name, float poseAt, float animTime)
{
	//Change animation, it may fail
	_animation = _ref.getAnimation(name);
	if (_animation)
	{
		for (unsigned int i = 0; i < _channel.size(); i++)
			_channel[i].poseAnimation(_elapAnimTime, &_animation->operator[](i), poseAt, animTime);
		_elapAnimTime = 0.f;
		_animDuration = animTime;
		_endState = Once;
		return true;
	}
	//No animation found, set current to null.
	stopAnimation();
	return false;
}
/* Freeze the current animation */
void AnimatedSkeleton::stopAnimation()
{
	_animation = nullptr;
	_endState = None;
}

/* Recalculates the end time when animation is looped */
void AnimatedSkeleton::loopRefit(){
	_elapAnimTime = (float)fmod((double)_elapAnimTime, (double)_animation->_duration); //loop
	for (unsigned int i = 0; i < boneCount(); i++)
		_channel[i].loop(_elapAnimTime, _animation->_duration);
}


const std::vector<glm::mat4>& AnimatedSkeleton::getSkin() const {
	return _skinTransform;
}
unsigned int AnimatedSkeleton::boneCount() const {
	return _pose.size();
}

/* Get the world matrix of a bone
*/
glm::mat4 AnimatedSkeleton::getBoneWorld(unsigned int bone) const
{
	return _root.getModelMatrix() * _pose[bone];
}


/* Create a bone node for the skeleton */
BoneNode* AnimatedSkeleton::getBoneNode(const std::string &bone) const
{
	unsigned int id;
	if (_ref.getBoneIndex(bone, id))
	{
		std::unique_ptr<BoneNode> node( new BoneNode(*this, id));
		_boneNodes.push_back(std::move(node));
		return _boneNodes.back().get();
	}
	return nullptr;
}