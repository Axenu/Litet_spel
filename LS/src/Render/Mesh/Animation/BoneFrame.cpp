#include "Render/Mesh/Animation/BoneFrame.h"
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>

BoneFrame::BoneFrame()
	: KeyFrame<3>(SIZE_BONE_CHANNEL)
{
}

BoneFrame::~BoneFrame()
{
}


void BoneFrame::storeState(unsigned int type, float eT) {
	if (type == 0)
		_from[type].set(lerpVec3(_from[type], _to[type], lerpAmount(type, eT)));
	else if(type == 1)
		_from[type].set(lerpVec3(_from[type], _to[type], lerpAmount(type, eT)));
	else
		_from[type].set(slerp(_from[type], _to[type], lerpAmount(type, eT)));
}

glm::mat4 BoneFrame::lerp(float eT) {
	//Calc lerp amount
	float amount[3];
	for (unsigned int i = 0; i < NUM_BONE_CHANNEL; i++)
		amount[i] = lerpAmount(i, eT);
	//Transform between the channel values
	glm::mat4 mat;
	mat = glm::scale(mat, lerpVec3(_from[1], _to[1], amount[1]));
	mat *= glm::mat4_cast(slerp(_from[2], _to[2], amount[2]));
	mat[3] = glm::vec4(lerpVec3(_from[0], _to[0], amount[2]), 1.f);
	return mat;
}