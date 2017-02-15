#pragma once
#include<vector>
#include "node.h"
#include "Bone.h"
#include "Skeleton.h"

class AnimatedSkeleton
{
private:
	Skeleton& _ref;
	Node *_root;
	Animation *_animation;
	/* From and to keys determine the currently lerped animations executed on each bone.
	 * To create a  
	 */
	std::vector<KeyFrame> _fromKeys;
	std::vector<KeyFrame> _toKeys;
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
	/* Update a bone with a new key from the current animation */
	void nextKey(int bone);
	/* Loop animation for the keys related to the bone */
	void loopKey(int bone);
public:
	AnimatedSkeleton(Skeleton& ref);
	~AnimatedSkeleton();

	void update(float dT);
	const std::vector<glm::mat4>& getSkin() const;
	unsigned int boneCount() const;
};

