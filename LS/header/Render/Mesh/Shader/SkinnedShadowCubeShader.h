#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "ShadowCubeShader.h"


class SkinnedShadowCubeShader
	: public ShadowCubeShader
{
private:

	GLint _boneUniform;
	void acquireUniforms();

public:
	SkinnedShadowCubeShader();
	virtual ~SkinnedShadowCubeShader();

	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *material) const;
};
