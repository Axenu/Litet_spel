#include "Render/Composition/RenderCubeMap.h"

RenderCubeMap::RenderCubeMap(const gl::RenderQuad& quad, EventManager &manager)
	: QuadShader(quad, "Quad", "Cube_Map_Buffer"), _curBuffer(manager, GLFW_KEY_5, GLFW_KEY_6, 0, 5)
{
	acquireUniforms();
}


RenderCubeMap::~RenderCubeMap()
{
}
bool RenderCubeMap::acquireUniforms()
{

	_shader.bind();
	_bufferID = _shader.getUniform("bufferID");

	//Bind samplers
	if (!_shader.bindSampler("shadowCubeMap", 0))	return false;
	return true;
}

/* Assign frame uniforms
*/
void RenderCubeMap::assignUniforms(RenderInfo &fD)
{
	/*	Bind resources
	*/
	fD._resource.getCubeMap(2).bindTextures();
	glUniform1ui(_bufferID, _curBuffer._value);
}
