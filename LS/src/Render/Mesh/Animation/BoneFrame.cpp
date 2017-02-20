#include "Render/Mesh/Animation/BoneFrame.h"
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

BoneFrame::BoneFrame()
	: KeyFrame<3>(SIZE_BONE_CHANNEL)
{
}

BoneFrame::~BoneFrame()
{
}


void BoneFrame::init(const glm::mat4& pose) {
	glm::vec3 pos, scale, skew;
	glm::quat rot;
	glm::vec4 perspective;
	glm::decompose(pose, scale, rot, pos, skew, perspective);
	_from[0].set(0.f, pos);
	_from[1].set(0.f, scale);
	_from[2].set(0.f, rot);
	_to[0].set(FLT_MAX, pos);
	_to[1].set(FLT_MAX, scale);
	_to[2].set(FLT_MAX, rot);
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
	checkUpdate(eT);
	//Calc lerp amount
	float amount[3];
	for (unsigned int i = 0; i < NUM_BONE_CHANNEL; i++)
		amount[i] = lerpAmount(i, eT);
	//Transform between the channel values
	glm::mat4 mat;
	mat = glm::scale(mat, lerpVec3(_from[1], _to[1], amount[1]));
	mat = glm::mat4_cast(slerp(_from[2], _to[2], amount[2])) * mat;
	mat[3] = glm::vec4(lerpVec3(_from[0], _to[0], amount[0]), 1.f);

	return mat;
}