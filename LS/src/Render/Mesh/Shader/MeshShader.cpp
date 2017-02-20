#include "Render/Mesh/Shader/MeshShader.h"

MeshShader::MeshShader(const std::string &shader)
	: _shader(shader){

}
MeshShader::MeshShader(const std::string &vertShader, const std::string &fragShader)
	: _shader(vertShader, fragShader) 
{

}

MeshShader::MeshShader(Shader &shader)
	: _shader(std::move(shader))
{
}


MeshShader::~MeshShader()
{
}
