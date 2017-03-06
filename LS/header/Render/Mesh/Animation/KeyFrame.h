#pragma once
/* Mattias F 2017*/
#include <vector>
#include <cfloat>
#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ChannelKeys.h"
#include "Channel.h"
#include "Animation.h"

/* Lerp the keys as vec3 with the specific amount */
glm::vec3 lerpVec3(const ChannelKey& from, const ChannelKey& to, float amount);
/* Slerp the keys as quaternions with the specific amount */
glm::quat slerp(const ChannelKey& from, const ChannelKey& to, float amount);

/* Contains the node's currently active keys for a node.
 * Allowing each node to maintain an individual
*/
template<unsigned int N>
class KeyFrame
{
private:
	/* Keep tracks on when the newest key needs an update */
	float _nextUpdate;
protected:
	/* Currently active channel */
	const Channel *_channel;
	/* Determines if the to keys belong to a custom channel animation and should persist over loop/change */
	bool _custom[N];
	/* The last key frames.
	*/
	ChannelMemoryKey _from[N];
	/* The next key frames.
	*/
	ChannelMemoryKey _to[N];
private:
	/* Find and set the next to key */
	void nextKey(unsigned int type, float eT);
	/* Find and set the previous key of the animation */
	void previousKey(unsigned int chnlInd, float eT);
	/* Find and set a the first key of the animation */
	void newKey(unsigned int chnlInd, float startAT);
	/* Update each key necesary */
	void updateFrame(float eT);
	/* Adjusts a custom frame over a animation change */
	void keepCustom(unsigned int chnlInd, float eT);
protected:
	/* Update frame if necesary */
	void checkUpdate(float eT);
	/* Get the lerp amount for a specific node channel */
	float lerpAmount(unsigned int type, float eT);
	/* Store the current animation state in the specific node channel
	*/
	virtual void storeState(unsigned int type, float eT) = 0;
public:
	KeyFrame();
	KeyFrame(const unsigned int* nodeChannelSize);
	~KeyFrame();
	bool isActive();
	/* Deactivate the channel, */
	void deactivate();
	/* Set new animation in the frame. Specify the animation time to start from */
	void newAnimation(float oldET, const Channel *channel, float startAt);
	/* Pose the animation at a specified frame. Blend between the current anim pose over the animation time param.*/
	void poseAnimation(float oldET, const Channel *channel, float poseAt, float animTime);
	/* Loop the animation */
	void loop(float eT, float animDuration);
};


template<unsigned int N>
KeyFrame<N>::KeyFrame()
{
	for (unsigned int i = 0; i < N; i++)
		_custom[i] = false;
}
template<unsigned int N>
KeyFrame<N>::KeyFrame(const unsigned int* nodeChannelSize)
	: _to(), _from()
{
	for (unsigned int i = 0; i < N; i++)
	{
		_to[i] = ChannelMemoryKey(nodeChannelSize[i]);
		_from[i] = ChannelMemoryKey(nodeChannelSize[i]);
		_custom[i] = false;
	}
}

template<unsigned int N>
KeyFrame<N>::~KeyFrame()
{
}

template<unsigned int N>
float KeyFrame<N>::lerpAmount(unsigned int type, float eT) {
	float delta = (_to[type]._time - _from[type]._time);
	return delta <= 0 ? 1.f : (eT - _from[type]._time) / delta;
}
template<unsigned int N>
void KeyFrame<N>::deactivate() {
	_channel = nullptr;
}
template<unsigned int N>
bool  KeyFrame<N>::isActive() {
	return _channel;
}
template<unsigned int N>
void KeyFrame<N>::newAnimation(float oldET, const Channel *channel, float startAt) {
	_channel = channel;
	if (_channel) {
		for (unsigned int i = 0; i < N; i++) {
			//Store current anim pose
			storeState(i, oldET);
			_from[i]._time = startAt;
			//Fetch the first key of the animation
			if (_custom[i])
				keepCustom(i, oldET);
			else
				newKey(i, startAt);
			//First frame to update keys
			_nextUpdate = std::fmin(_nextUpdate, _to[i]._time);
		}
	}
}

/* Pose the animation at a specified frame. Blend between the current anim pose over the animation time param.*/
template<unsigned int N>
void KeyFrame<N>::poseAnimation(float oldET, const Channel *channel, float poseAt, float animTime)
{
	_channel = channel;
	if (_channel) {
		for (unsigned int i = 0; i < N; i++) {
			//Store current anim pose
			storeState(i, oldET);
			//Temporarily store state
			ChannelMemoryKey tmpCpy(_from[i]);
			//Fetch and create pose keys
			newKey(i, poseAt);
			previousKey(i, poseAt);
			storeState(i, poseAt);
			//Setup the animation, note that this is not a 'custom' animation as it affects all channels.
			_to[i] = _from[i];
			_from[i] = tmpCpy;
			_from[i]._time = 0.f;
			_to[i]._time = animTime;
		}
		_nextUpdate = FLT_MAX;
		_channel = nullptr;
	}
}
template<unsigned int N>
void  KeyFrame<N>::checkUpdate(float eT) {
	//Update keys if necesary
	if (_nextUpdate < eT)
		updateFrame(eT);
}

template<unsigned int N>
void  KeyFrame<N>::updateFrame(float eT) {
	float time = FLT_MAX;
	for (unsigned int i = 0; i < N; i++) {
		if (_to[i]._time < eT)
		{
			_from[i] = _to[i]; //Copy next -> last
			nextKey(i, eT);
		}
		//Update
		time = std::fmin(time, _to[i]._time);
	}
}
template<unsigned int N>
void  KeyFrame<N>::nextKey(unsigned int type, float eT) {
	int key = _channel->getNext(eT, type);
	if (key != -1) //If key is found, set it
		_to[type] = _channel->getKey(key, type);
	else {
		_to[type]._time = FLT_MAX; //Let the frame elapse until the animation end
		_custom[type] = false; //This is not a custom key.
	}
}
template<unsigned int N>
void  KeyFrame<N>::previousKey(unsigned int chnlInd, float eT)
{
	int key = _channel->getPrevious(eT, chnlInd);
	if (key != -1) //If key is found, set it
		_from[chnlInd] = _channel->getKey(key, chnlInd);
	else 
		_from[chnlInd]._time = eT; //Animate from current value (and time)
}
/* Find and set a the first key of the animation */
template<unsigned int N>
void  KeyFrame<N>::newKey(unsigned int chnlInd, float startAT)
{
	int key = _channel->getNext(startAT, chnlInd);
	if (key != -1) //If key is found, set it
		_to[chnlInd] = _channel->getKey(key, chnlInd);
	else {
		//There is no key in the animation channel's , set the current value to continue to the endtimes.
		_to[chnlInd] = _from[chnlInd];
		_to[chnlInd]._time = FLT_MAX;
		_custom[chnlInd] = false;
	}
}

/* Adjusts a custom frame over a animation change */
template<unsigned int N>
void  KeyFrame<N>::keepCustom(unsigned int chnlInd, float eT)
{
	//Let custom frame persist, adjusting the time relation
	_from[chnlInd]._time -= eT;
	_to[chnlInd]._time -= eT;
}

template<unsigned int N>
void  KeyFrame<N>::loop(float eT, float animDuration) {
	for (unsigned int i = 0; i < N; i++)
	{
		if (_custom[i])
			keepCustom(i, animDuration);
		else {
			//Animate from last frame to first
			_from[i] = _to[i];
			_from[i]._time = 0.f;
			nextKey(i, eT);
		}
	}
}
