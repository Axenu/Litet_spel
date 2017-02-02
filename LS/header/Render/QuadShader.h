#pragma once

#include"Shader.h"
#include"gl/RenderQuad.h"
#include"RenderInfo.h"

class QuadShader
{
protected:
	Shader _shader;
	const gl::RenderQuad& _quad;

	virtual void assignUniforms(RenderInfo &fD) = 0;

public:
	QuadShader(const gl::RenderQuad& quad, Shader &shader);
	QuadShader(const gl::RenderQuad& quad, const std::string &vertShader, const std::string &fragShader);
	virtual ~QuadShader();


	void render(RenderInfo &fD);
};
