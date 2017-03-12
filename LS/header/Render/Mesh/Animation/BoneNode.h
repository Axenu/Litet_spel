#pragma once
#include "node.h"

/* Forward Declare
*/
class AnimatedSkeleton;

class BoneNode :
	public Node
{
private:
	/* Reference to the skeleton */
	const AnimatedSkeleton *_ref;
	/* Bone parent */
	unsigned int _bone;
public:
	BoneNode(const AnimatedSkeleton &skel, unsigned int bone);
	virtual ~BoneNode();

	virtual void update(float dT);
};

