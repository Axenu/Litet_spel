#pragma once
#include "../QuadShader.h"
#include "gl/GLFunctions.h"
#include "Game/camera.h"
#include "Event/Input/InputIncrementer.h"


class RenderCubeMap :
	public QuadShader
{
	InputIncrementer _curBuffer;
	//Calculation uniforms:
	GLint  _bufferID;

protected:

	virtual bool acquireUniforms();
	virtual void assignUniforms(RenderInfo &fD);

public:
	RenderCubeMap(const gl::RenderQuad& quad, EventManager *manager);
	virtual ~RenderCubeMap();

	/* Call on window size change
	*/
	// void setWindowSize(float wWidth, float wHeight, const Camera &camera);
};
