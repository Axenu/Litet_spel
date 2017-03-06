#pragma once
#include <vector>
#include <math.h>
#include "node.h"
#include "Bone.h"
#include "Skeleton.h"
#include "BoneFrame.h"

class AnimatedSkeleton
{
public:
	enum PlayType
	{
		None = 0,
		Once = 1,
		Loop = 2
	};
private:
	Skeleton& _ref;
	Node& _root;
	Animation *_animation;
	/* Current keys active in the animation for each channel
	 */
	std::vector<BoneFrame> _channel;
	/* Time elapsed time of the running animation */
	float _elapAnimTime;
	/* Local animation duration */
	float _animDuration;
	PlayType _endState;
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
	bool setAnim(const std::string& name, PlayType runType);
	/* Pose the skeleton at the specific point of animation. Blends into the pose over the animation time specified. */
	bool setAnimPose(const std::string& name, float poseAt, float animTime);
	/* Freeze the current animation. */
	void stopAnimation();
	/* Get skin matrixes
	*/
	const std::vector<glm::mat4>& getSkin() const;
	/* Get the number of bones
	*/
	unsigned int boneCount() const;

	/* Get the world matrix of a bone
	*/
	glm::mat4 getBoneWorld(unsigned int bone);
};
