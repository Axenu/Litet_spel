#pragma once
#include "../QuadShader.h"
#include "gl/GLFunctions.h"
#include "Game/camera.h"
#include "Event/Input/InputIncrementer.h"


	/* The shader implementation rendering the pre-generated
	deferred buffers and other shading data (shadowmaps) for the final composition.

	*/
class RenderDefBuffers :
	public QuadShader
{
	InputIncrementer _curBuffer;
	//Calculation uniforms:
	GLint  _bufferID, _screenInv, _near, _far, _right, _top;

protected:

	virtual bool acquireUniforms();
	virtual void assignUniforms(RenderInfo &fD);

public:
	RenderDefBuffers(const gl::RenderQuad& quad, EventManager *manager);
	virtual ~RenderDefBuffers();

	/* Call on window size change
	*/
	void setWindowSize(float wWidth, float wHeight, const Camera &camera);
};
