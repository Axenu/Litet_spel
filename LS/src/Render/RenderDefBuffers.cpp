#include "Render/RenderDefBuffers.h"

RenderDefBuffers::RenderDefBuffers(const gl::RenderQuad& quad, EventManager &manager)
	: QuadShader(quad, "Quad", "Deferred_Buffer"), _curBuffer(manager, GLFW_KEY_KP_ADD, GLFW_KEY_KP_SUBTRACT, 0, 8)
{
	acquireUniforms();
}


RenderDefBuffers::~RenderDefBuffers()
{
}
bool RenderDefBuffers::acquireUniforms() {

	_shader.bind();
	/*Function params
	*/
	//Pos inverse
	_screenInv = _shader.getUniform("screenInv");
	_near = _shader.getUniform("near");
	_far = _shader.getUniform("far");
	_top = _shader.getUniform("top");
	_right = _shader.getUniform("right");


	//Point light
	_bufferID = _shader.getUniform("bufferID");

	//Bind samplers
	if (!_shader.bindSampler("colBuffer", 0))	return false;
	if (!_shader.bindSampler("norBuffer", 1))	return false;
	if (!_shader.bindSampler("specBuffer", 2))	return false;
	if (!_shader.bindSampler("depthBuffer", 3))	return false;
	return true;
}

/* Assign frame uniforms
*/
void RenderDefBuffers::assignUniforms(RenderInfo &fD) {
	/*	Bind resources
	*/
	fD._resource.getDeffered().bindTextures();
	glUniform1ui(_bufferID, _curBuffer._value);
}

/* Call on window size change
*/
void RenderDefBuffers::setWindowSize(float wWidth, float wHeight, const Camera &camera) {
	_shader.bind();
	glUniform2f(_screenInv, 1.0f / wWidth, 1.0f / wHeight);
	glUniform1f(_near, camera.getNearPlane());
	glUniform1f(_far, camera.getFarPlane());
	float halfTanFowy = std::tan(camera.getFieldOfView() * 0.5f);
	glUniform1f(_top, halfTanFowy);
	glUniform1f(_right, halfTanFowy * camera.getAspectRatio());
}