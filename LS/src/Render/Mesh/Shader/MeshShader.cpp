#include "Render/Mesh/Shader/MeshShader.h"

MeshShader::MeshShader(const std::string &shader)
	: _shader(shader)
{

}

MeshShader::MeshShader(Shader &shader)
	: _shader(std::move(shader))
{
}


MeshShader::~MeshShader()
{
}
