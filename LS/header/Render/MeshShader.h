#pragma once

#include<string>
#include"Shader.h"
#include"RenderInfo.h"

class MeshShader
{
protected:
	Shader _shader;

public:
	MeshShader(const std::string &shader);
	MeshShader(Shader &shader);
	virtual ~MeshShader();
	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix) = 0;
};

