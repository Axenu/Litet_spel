#include "Render/DeferredMeshShader.h"



DeferredMeshShader::DeferredMeshShader()
	: MeshShader("Deferred_Mesh")
{
	acquireUniforms();
}


DeferredMeshShader::~DeferredMeshShader()
{
}

void DeferredMeshShader::acquireUniforms() {
	_mvp = _shader.getUniform("mvp");
	_mv = _shader.getUniform("mv");
	_difCol = _shader.getUniform("diffuseCol");
	_specCol = _shader.getUniform("specularCol");
}



/* Bind shader and assign related uniforms
*/
void DeferredMeshShader::assignUniforms(FrameData &fD, const glm::mat4 &modelMatrix) {
	_shader.bind();
	//Todo add game object transforms
	glm::mat4 mvp = fD._VP * modelMatrix;
	glm::mat4 mv = fD._V * modelMatrix;
	glUniformMatrix4fv(_mvp, 1, GL_FALSE, (const GLfloat*)&(mvp));
	glUniformMatrix4fv(_mv, 1, GL_FALSE, (const GLfloat*)&(mv));
	glUniform3f(_difCol, 0.8f, 0.8f, 0.8f);
	glUniform3f(_specCol, 1.0f, 1.0f, 1.0f);
}