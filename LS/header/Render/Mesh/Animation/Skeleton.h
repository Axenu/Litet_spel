#pragma once
#include <vector>
#include <map>
#include "node.h"
#include "Bone.h"
#include "Animation.h"

class Skeleton
{
	std::vector<Bone> _bones;
	std::map<std::string, Animation> _animation;

public:
	Skeleton(std::vector<Bone> &bones);
	~Skeleton();

	Animation* getAnimation(const std::string &name);
	std::vector<Bone> getBones();
};

