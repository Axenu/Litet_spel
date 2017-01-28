#include "Render/QuadShader.h"

QuadShader::QuadShader(const gl::RenderQuad& quad)
	: _quad(quad)
{
}

QuadShader::QuadShader(const gl::RenderQuad& quad, Shader &shader)
	: _quad(quad), _shader(std::move(shader))
{
}


QuadShader::~QuadShader()
{
}


void QuadShader::render(FrameData &fD) {
	_shader.bind();
	assignUniforms(fD);
}
