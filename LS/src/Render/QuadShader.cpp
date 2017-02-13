#include "Render/QuadShader.h"

QuadShader::QuadShader(const gl::RenderQuad& quad, Shader &shader)
	: _quad(quad), _shader(std::move(shader))
{
}
QuadShader::QuadShader(const gl::RenderQuad& quad, const std::string &vertShader, const std::string &fragShader)
	: _quad(quad), _shader(vertShader, fragShader) {

}


QuadShader::~QuadShader()
{
}


void QuadShader::render(RenderInfo &fD) {
	_shader.bind();
	assignUniforms(fD);
	_quad.render();
}
