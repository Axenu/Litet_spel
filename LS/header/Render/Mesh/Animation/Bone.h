#pragma once
#include<string>
#include<glm/mat4x4.hpp>


struct Bone {

	std::string _boneName;
	glm::mat4 _invBindPose;
	glm::mat4 _bindPose;
	int _boneIndex;
	/* Parent bone */
	int _parentIndex;
	Bone()
		: _boneName(), _invBindPose(), _bindPose(), _boneIndex(-1), _parentIndex(-1)
	{
	}
	Bone(const std::string &boneName, const glm::mat4 &invBindPose, glm::mat4 &bindPose, int boneIndex, int parentIndex) 
		: _boneName(boneName), _invBindPose(invBindPose), _bindPose(bindPose), _boneIndex(boneIndex), _parentIndex(parentIndex)
	{
	}
};