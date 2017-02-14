#pragma once
#include<vector>
#include "node.h"
#include "Bone.h"

class Skeleton
{
	Node *_root;
	std::vector<Bone> _bones;
	std::vector<glm::mat4> _animation;
	/* Bone transform in model space
	*/
	std::vector<glm::mat4> _pose;
	/* Transformation from rest pose -> world space.
	*/
	std::vector<glm::mat4> _skinTransform;

public:
	Skeleton(std::vector<Bone> &bones);
	~Skeleton();

	void update(float dT);
	const std::vector<glm::mat4>& getSkin() const;
};

