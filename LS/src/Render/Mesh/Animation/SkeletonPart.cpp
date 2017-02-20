#include "Render/Mesh/Animation/SkeletonPart.h"



SkeletonPart::SkeletonPart()
	: _bones()
{
}
SkeletonPart::SkeletonPart(std::vector<int> &bones) 
	: _bones(bones) 
{
}


SkeletonPart::~SkeletonPart()
{
}

/* Assemble the mat4 uniform array from the specific skeleton parent
*/
void SkeletonPart::assemble(const AnimatedSkeleton &parent, glm::mat4* array) const {
	for (unsigned int i = 0; i < _bones.size(); i++)
		array[i] = parent.getSkin()[_bones[i]];
}

bool SkeletonPart::hasSkeleton() const {
	return _bones.size() > 0;
}
unsigned int SkeletonPart::numBones() const {
	return _bones.size();
}
