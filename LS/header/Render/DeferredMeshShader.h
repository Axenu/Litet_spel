#pragma once
#include <glm/glm.hpp>
#include "MeshShader.h"


class DeferredMeshShader
	: public MeshShader
{
private:

	GLint _mvpUniform, _mvUniform, _difColUniform, _specColUniform;
	glm::vec3 _difCol, _specCol;
	void acquireUniforms();

public:	
	DeferredMeshShader();
	virtual ~DeferredMeshShader();
	void setDif(float r, float g, float b);
	void setSpec(float r, float g, float b);

	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix);
};

