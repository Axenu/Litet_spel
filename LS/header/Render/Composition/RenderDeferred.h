#pragma once
#include "../QuadShader.h"
#include "gl/GLFunctions.h"
#include "Game/camera.h"
#include <glm/gtx/string_cast.hpp>


	/* The shader implementation rendering the pre-generated
	deferred buffers and other shading data (shadowmaps) for the final composition.

	*/
class RenderDeferred :
	public QuadShader
{
	/* Maximum number of lights available to render.
	*/
	const static unsigned int MAXLIGHTCOUNT = 8;

	//Calculation uniforms:
	GLint _screenInv, _near, _far, _right, _top, _viewMatrixLocation, _camPosLocation;
	//Light uniforms
	GLint _pNumLights, _pLightPos, _pLightFade, _pLightSpecCol, _pLightDiffCol;
	//Other
	GLint _lightNade;

protected:

	virtual bool acquireUniforms();
	virtual void assignUniforms(RenderInfo &rI);

public:
	RenderDeferred(const gl::RenderQuad& quad);
	virtual ~RenderDeferred();

	/* Call on window size change
	*/
	void setWindowSize(float wWidth, float wHeight, const Camera &camera);
};
