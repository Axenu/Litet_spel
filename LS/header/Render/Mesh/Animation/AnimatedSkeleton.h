#pragma once
#include <vector>
#include <math.h>
#include "node.h"
#include "Bone.h"
#include "Skeleton.h"
#include "BoneFrame.h"

class AnimatedSkeleton
{
private:
	Skeleton& _ref;
	Node& _root;
	Animation *_animation;
	/* Current keys active in the animation for each channel
	 */
	std::vector<BoneFrame> _channel;
	/* Time elapsed time of the running animation */
	float _elapAnimTime;
	/* Bone transform in model space
	*/
	std::vector<glm::mat4> _pose;
	/* Transformation from rest pose -> world space.
	*/
	std::vector<glm::mat4> _skinTransform;

	/* Recalculates the end time when animation is looped */
	void loopRefit();
public:
	AnimatedSkeleton(Skeleton& ref, Node& root);
	~AnimatedSkeleton();

	void update(float dT);
	void updateSkeleton();
	/* Set the animation from the name, returns true if animation found */
	bool setAnim(const std::string& name);
	const std::vector<glm::mat4>& getSkin() const;
	unsigned int boneCount() const;
};
