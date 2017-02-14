#pragma once
#include<string>
#include<glm/mat4x4.hpp>


struct Bone {

	std::string _boneName;
	glm::mat4 _invBindPose;
	int _boneIndex;
	/* Parent bone */
	int _parentIndex;
	Bone()
		: _boneName(), _invBindPose(), _boneIndex(-1), _parentIndex(-1)
	{
	}
	Bone(const std::string &boneName, const glm::mat4 &invBindPose, int boneIndex, int parentIndex) 
		: _boneName(boneName), _invBindPose(invBindPose), _boneIndex(boneIndex), _parentIndex(parentIndex)
	{
	}
};