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
	const std::vector<Bone>& getBones() const;
	const Bone& getBone(unsigned int i) const;
	/* Access a bone from name. Returns if bone name exists.
	boneName	<<	Name matched with the bones.
	boneID		>>	Returned bone index if found.
	return		>>	Returns true if bone was found.
	*/
	bool getBoneIndex(const std::string &boneName, unsigned int &boneID) const;
	/* Get the number of bones in the skeleton */
	unsigned int getNumBones() const;
	/* Add an animation to the skeleton */
	void addAnimation(std::shared_ptr<Animation> &anim);
};

