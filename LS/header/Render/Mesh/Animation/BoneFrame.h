#pragma once
#include "KeyFrame.h"

class BoneFrame
	: public KeyFrame<3>
{
	const unsigned int NUM_BONE_CHANNEL = 3;
	const unsigned int SIZE_BONE_CHANNEL[3] = { 3, 3, 4 };
public:
	BoneFrame();
	~BoneFrame();
	/* Lerps from the keyframe to the next one. Updating the active keys.
	*/
	glm::mat4 lerp(float eT);
	/* Store the current animation state in the specific node channel
	*/
	virtual void storeState(unsigned int type, float eT);
};

