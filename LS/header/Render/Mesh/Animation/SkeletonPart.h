#pragma once
#include <vector>
#include "AnimatedSkeleton.h"

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
	SkeletonPart(std::vector<int> &bones);
	~SkeletonPart();
	/* Assemble the mat4 uniform array from the specific skeleton parent
	*/
	void assemble(const AnimatedSkeleton &parent, glm::mat4* array) const;
	/* Assemble the mat4 uniform array from the specific skeleton parent, applies the modelmatrix to the calculation.
	*/
	void assemble(const AnimatedSkeleton &parent, const glm::mat4 &modelMatrix, glm::mat4* array) const;
	bool hasSkeleton() const;
	unsigned int numBones() const;
};

