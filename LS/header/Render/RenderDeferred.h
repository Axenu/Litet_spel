#pragma once
#include"QuadShader.h"
#include"gl\GLFunctions.h"
#include"camera.h"


	/* The shader implementation rendering the pre-generated 
	deferred buffers and other shading data (shadowmaps) for the final composition.
	
	*/
class RenderDeferred :
	public QuadShader
{
	//Calculation uniforms:
	GLint _screenInv, _near, _far, _right, _top;
	//Light uniforms
	GLint _pLightPos, _pLight_fade, _pLightSpecCol, _pLightDiffCol;

protected:

	virtual bool acquireUniforms();
	virtual void assignUniforms(FrameData &fD);

public:
	RenderDeferred(const gl::RenderQuad& quad);
	~RenderDeferred();

	/* Call on window size change
	*/
	void setWindowSize(float wWidth, float wHeight, const Camera &camera);
};

