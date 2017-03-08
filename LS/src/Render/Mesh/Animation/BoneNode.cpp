#include "Render/Mesh/Animation/BoneNode.h"
#include "Render/Mesh/Animation/AnimatedSkeleton.h"



BoneNode::BoneNode(const AnimatedSkeleton &skel, unsigned int bone)
	: _ref(skel), _bone(bone)
{
}


BoneNode::~BoneNode()
{
}

void BoneNode::update(float dT)
{
	calcModelMatrix();
	_modelMatrix = _ref.getBoneWorld(_bone) * _modelMatrix;
	updateChildren(dT);
}