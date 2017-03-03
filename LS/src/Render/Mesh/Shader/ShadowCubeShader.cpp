#include "Render/Mesh/Shader/ShadowCubeShader.h"

ShadowCubeShader::ShadowCubeShader()
	: ShadowCubeShader("shadow_cube_map") {}
ShadowCubeShader::ShadowCubeShader(const std::string &vertShader)
	: MeshShader(vertShader, "shadow_cube_map", "shadow_cube_map")
{
	acquireUniforms();
}


ShadowCubeShader::~ShadowCubeShader()
{
}

void ShadowCubeShader::acquireUniforms()
{
	_mmUniform = _shader.getUniform("model");
	_shadowMatricesLocation = _shader.getUniform("shadowMatrices");
	_lightPosLocation = _shader.getUniform("lightPos");
	_farPlaneLocation = _shader.getUniform("far_plane");
}


/* Sets up the cube shader for rendering to a specific point light
*/
void ShadowCubeShader::setUp(const PointLight &pointL)
{
	_shader.bind();
	glUniformMatrix4fv(_shadowMatricesLocation, 6, GL_FALSE, &pointL._shadowMatrices[0][0][0]);
	glUniform3fv(_lightPosLocation, 1, &pointL._light._pos[0]);
	glUniform1f(_farPlaneLocation, pointL._light._fadeDist);

}

/* Bind shader and assign related uniforms
*/
void ShadowCubeShader::assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *matLink) const {

	//_shader.bind(); - Do not bind! Shader used for batched rendering only and already bound

	//Send matrixes
	glUniformMatrix4fv(_mmUniform, 1, GL_FALSE, &modelMatrix[0][0]);
}


/* Link the shader to the material
*/
std::shared_ptr<MaterialLink> ShadowCubeShader::linkMaterial(Material &mat) const {

	return std::shared_ptr<MaterialLink>(nullptr);
}