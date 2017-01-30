#include "Render/RenderDeferred.h"

	RenderDeferred::RenderDeferred(const gl::RenderQuad& quad)
		: QuadShader(quad, "Quad", "Deferred_Comp")
	{
		acquireUniforms();
	}


	RenderDeferred::~RenderDeferred()
	{
	}
	bool RenderDeferred::acquireUniforms(){

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
		_pLight_fade = _shader.getUniform("pLight_Fade");
		_pLightPos = _shader.getUniform("pLightPos");
		_pLightSpecCol = _shader.getUniform("pLightspecCol");
		_pLightDiffCol = _shader.getUniform("pLightDiffCol");


		//Bind samplers
		if (!_shader.bindSampler("colBuffer", 0))	return false;
		if (!_shader.bindSampler("norBuffer", 1))	return false;
		if (!_shader.bindSampler("specBuffer", 2))	return false;
		if (!_shader.bindSampler("depthBuffer", 3))	return false;
		return true;
	}

	/* Assign frame uniforms
	*/
	void RenderDeferred::assignUniforms(FrameData &fD){
		//Bind resources
		fD._resource.getDeffered().bindTextures();

		/*	Set point light
		*/
		glm::vec3 lightPos = fD._V * glm::vec4(0.0f, -6.0f, 2.0f, 1.0f);
		glUniform3f(_pLightPos, lightPos.x, lightPos.y, lightPos.z);
		glUniform1f(_pLight_fade, 5.0f);
		glUniform3f(_pLightDiffCol, 0.8f, 0.8f, 0.8f);
		glUniform3f(_pLightSpecCol, 0.0f, 1.0f, 0.0f);
	}

	/* Call on window size change
	*/
	void RenderDeferred::setWindowSize(float wWidth, float wHeight, const Camera &camera){
		glUniform2f(_screenInv, 1.0f / wWidth, 1.0f / wHeight);
		glUniform1f(_near, camera.getNearPlane());
		glUniform1f(_far, camera.getFarPlane());
		float halfTanFowy = std::tan(camera.getFieldOfView() * 0.5f);
		glUniform1f(_top, halfTanFowy);
		glUniform1f(_right, halfTanFowy * camera.getAspectRatio());
	}
