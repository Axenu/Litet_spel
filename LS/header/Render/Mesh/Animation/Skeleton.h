#pragma once
#include <vector>
#include <map>
#include "node.h"
#include "Bone.h"
#include "Animation.h"
#include <memory>
/* The skeleton consists of the bone data loaded into the game.
*/
class Skeleton
{
	/* Bone structure of the skeleton.
	*/
	std::vector<Bone> _bones;
	/* Animations related to the skeleton. 
	 * The animations is owned by the skeleton however it can be shared over multiple instances.
	*/
	std::map<std::string, std::shared_ptr<Animation>> _animation;

public:
	Skeleton(std::vector<Bone> &bones);
	~Skeleton();

	Animation* getAnimation(const std::string &name);
	std::vector<Bone> getBones();
	/* Add an animation to the skeleton */
	void addAnimation(std::shared_ptr<Animation> &anim);
};

