#pragma once

#include<string>
#include"Shader.h"
#include"FrameData.h"

class MeshShader
{
protected:
	Shader _shader;

public:
	MeshShader(const std::string &shader);
	MeshShader(Shader &shader);
	~MeshShader();
	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(FrameData &fD) = 0;
};

