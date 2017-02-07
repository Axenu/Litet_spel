#include "Render/Mesh/Shader/DeferredMeshShader.h"


DeferredMeshShader::DeferredMeshShader()
	: MeshShader("Deferred_Mesh")
{
	acquireUniforms();
}


DeferredMeshShader::~DeferredMeshShader()
{
}

void DeferredMeshShader::acquireUniforms() {
	_mvpUniform = _shader.getUniform("mvp");
	_mvUniform = _shader.getUniform("mv");
	_difColUniform = _shader.getUniform("diffuseCol");
	_specColUniform = _shader.getUniform("specularCol");
}



/* Bind shader and assign related uniforms
*/
void DeferredMeshShader::assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix, MaterialLink *matLink) {
	_shader.bind();
	//Todo add game object transforms
	glm::mat4 mvp = fD._VP * modelMatrix;
	glm::mat4 mv = fD._V * modelMatrix;
	glUniformMatrix4fv(_mvpUniform, 1, GL_FALSE, (const GLfloat*)&(mvp));
	glUniformMatrix4fv(_mvUniform, 1, GL_FALSE, (const GLfloat*)&(mv));
	
	DeferredMaterial *mat = dynamic_cast<DeferredMaterial*>(matLink);
	if (mat) {
		glUniform3f(_difColUniform, mat->_diffuse.r, mat->_diffuse.g, mat->_diffuse.b);
		glUniform4f(_specColUniform, mat->_specular.r, mat->_specular.g, mat->_specular.b, mat->_shininess);
	}
	else {
		glUniform3f(_difColUniform, 1.0f, 0.0f, 0.0f);
		glUniform4f(_specColUniform, 1.0f, 0.0f, 0.0f, 20.f);
	}
}


/* Link the shader to the material
*/
std::shared_ptr<MaterialLink> DeferredMeshShader::linkMaterial(Material &mat) {
	DeferredMaterial *data = new DeferredMaterial();
	if(!mat.tryGet("diffuse", data->_diffuse))
		data->_diffuse = glm::vec4(0.8f);
	if (!mat.tryGet("spec", data->_specular))
		data->_specular = glm::vec4(0.8f);
	if (!mat.tryGet("shine", data->_shininess))
		data->_shininess = 20.0f;

	return std::shared_ptr<MaterialLink>(dynamic_cast<MaterialLink*>(data));
}