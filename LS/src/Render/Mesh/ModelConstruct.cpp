#include "ModelConstruct.h"

ModelConstruct::ModelConstruct(MeshShader *shader)
	: _shader(shader), _parts(), _animations() {	}

ModelConstruct::~ModelConstruct() {}

/*	Get a bone index from string. -1 if nothing found */
int ModelConstruct::getBoneIndex(const std::string &name) {
	for (unsigned int i = 0; i < _bones.size(); i++)
	{
		if (name == _bones[i]._boneName)
			return i;
	}
	return -1;
}


Skeleton* ModelConstruct::getSkeleton() {
	Skeleton* skel = new Skeleton(_bones);
	for (unsigned int i = 0; i < _animations.size(); i++)
		skel->addAnimation(_animations[i]);
	return skel;
}