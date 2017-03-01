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
	/* Determines if the to keys are custom generated and should persist over loop/change */
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
	/* Update each key necesary */
	void updateFrame(float eT);
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
	/* Set new animation in the frame*/
	void newAnimation(float oldET, const Channel *channel, float animDur);
	/* Loop the animation */
	void loop(float eT, float animDuration);
};


template<unsigned int N>
KeyFrame<N>::KeyFrame()
{
}
template<unsigned int N>
KeyFrame<N>::KeyFrame(const unsigned int* nodeChannelSize)
	: _to(), _from()
{
	for (unsigned int i = 0; i < N; i++)
	{
		_to[i] = ChannelMemoryKey(nodeChannelSize[i]);
		_from[i] = ChannelMemoryKey(nodeChannelSize[i]);
	}
}

template<unsigned int N>
KeyFrame<N>::~KeyFrame()
{
}

template<unsigned int N>
float KeyFrame<N>::lerpAmount(unsigned int type, float eT) {
	return (eT - _from[type]._time) / (_to[type]._time - _from[type]._time);
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
void KeyFrame<N>::newAnimation(float oldET, const Channel *channel, float animDuration) {
	_channel = channel;
	if (_channel) {
		float time = FLT_MAX;
		for (unsigned int i = 0; i < N; i++) {
			storeState(i, oldET);
			_from[i]._time = 0.f;
			if (_channel && _channel->numKeys(i) > 0)
				nextKey(i, 0.f);
			else
			{	//If there is no values in the node's channel type keep current state
				_to[i] = _from[i];
				_to[i]._time = FLT_MAX;
			}
			//Update
			time = std::fmin(time, _to[i]._time);
		}
		_nextUpdate = time;
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
void  KeyFrame<N>::loop(float eT, float animDuration) {
	for (unsigned int i = 0; i < N; i++)
	{
		if (_custom[i]) {
			//Let custom frame persist, adjusting the time relation
			_from[i]._time = _from[i]._time - animDuration;
			_to[i]._time -= animDuration;
		}
		else {
			//Animate from last frame to first
			_from[i] = _to[i];
			_from[i]._time = 0.f;
			nextKey(i, eT);
		}
	}
}
