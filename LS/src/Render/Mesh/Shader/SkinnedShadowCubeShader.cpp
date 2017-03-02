#include "Render/Mesh/Shader/SkinnedShadowCubeShader.h"


SkinnedShadowCubeShader::SkinnedShadowCubeShader()
	: ShadowCubeShader("skinned_shadow_cube_map")
{
	acquireUniforms();
}


SkinnedShadowCubeShader::~SkinnedShadowCubeShader()
{
}

void SkinnedShadowCubeShader::acquireUniforms()
{
	_boneUniform = _shader.getUniform("bones");
}



/* Bind shader and assign related uniforms
*/
void SkinnedShadowCubeShader::assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *matLink) const {

	//_shader.bind(); - Do not bind! Shader used for batched rendering only and already bound

	//Assemble part bone matrix array:
	glm::mat4 BONES[MAX_BONE_COUNT];
	const SkeletonPart& skelPart = part.getMesh().getSkeletonPart();
	skelPart.assemble(*part.getSkeleton(), modelMatrix, BONES);
	glUniformMatrix4fv(_boneUniform, skelPart.numBones(), GL_FALSE, (const GLfloat*)BONES);
}