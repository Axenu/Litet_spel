#include "Render/DeferredMeshShader.h"


DeferredMeshShader::DeferredMeshShader()
	: MeshShader("Deferred_Mesh")
{
	acquireUniforms();
	this->setDif(0.8f, 0.8f, 0.8f);
	this->setSpec(1.0f, 1.0f, 1.0f);
}


DeferredMeshShader::~DeferredMeshShader()
{
}

void DeferredMeshShader::setDif(float r, float g, float b)
{
	_difCol.r = r;
	_difCol.g = g;
	_difCol.b = b;
}

void DeferredMeshShader::setSpec(float r, float g, float b)
{
	_specCol.r = r;
	_specCol.g = g;
	_specCol.b = b;
}

void DeferredMeshShader::acquireUniforms() {
	_mvpUniform = _shader.getUniform("mvp");
	_mvUniform = _shader.getUniform("mv");
	_difColUniform = _shader.getUniform("diffuseCol");
	_specColUniform = _shader.getUniform("specularCol");
}



/* Bind shader and assign related uniforms
*/
void DeferredMeshShader::assignUniforms(FrameData &fD, const glm::mat4 &modelMatrix) {
	_shader.bind();
	//Todo add game object transforms
	glm::mat4 mvp = fD._VP * modelMatrix;
	glm::mat4 mv = fD._V * modelMatrix;
	glUniformMatrix4fv(_mvpUniform, 1, GL_FALSE, (const GLfloat*)&(mvp));
	glUniformMatrix4fv(_mvUniform, 1, GL_FALSE, (const GLfloat*)&(mv));
	glUniform3f(_difColUniform, _difCol.r, _difCol.g, _difCol.b);
	glUniform3f(_specColUniform, _specCol.r, _specCol.g, _specCol.b);
}