#pragma once

#include"Shader.h"
#include"gl\RenderQuad.h"
#include"FrameData.h"
class QuadShader
{
protected:
	Shader _shader;
	const gl::RenderQuad& _quad;

	virtual void assignUniforms(FrameData &fD) = 0;

public:
	QuadShader(const gl::RenderQuad& quad, Shader &shader);
	~QuadShader();


	void render(FrameData &fD);
};

