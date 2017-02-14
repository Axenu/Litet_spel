#pragma once
#include <vector>
#include "Skeleton.h"

const int MAX_BONE_COUNT = 48;

/* Part of Skeleton related to a Mesh object.
*/
class SkeletonPart
{
private:
	/* The bones of the skeleton
	*/
	std::vector<int> _bones;
public:
	SkeletonPart();
	~SkeletonPart();
	/* Assemble the mat4 uniform array from the specific skeleton parent
	*/
	void assemble(const Skeleton &parent, glm::mat4* array);
	bool hasSkeleton();
};

