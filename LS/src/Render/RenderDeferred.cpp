#include "Render/RenderDeferred.h"
#include "Render/PointLight.h"

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
		_pNumLights = _shader.getUniform("pNumLight");
		_pLight = _shader.getUniform("pLight[0].pos");

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
		unsigned int lightCount = 2;
		PointLight lights[8];
		lights[0] = PointLight(fD._eye, glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
		lights[1] = PointLight(glm::vec3(0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(1.0f, 0.0f, 0.0f), 5.0f);
		//Transform positions in to viewspace
		for(unsigned int i = 0;i < lightCount; i++)
			lights[i]._pos = fD._V * glm::vec4(fD._eye, 1.0f);
		/*	Set point light
		*/
		unsigned int vec4Elems = lightCount * sizeof(PointLight) / (4 * 4);
		glUniform1ui(_pNumLights, lightCount);
		glUniform4fv(_pLight, vec4Elems, (const GLfloat*)&lights[0]);
	}

	/* Call on window size change
	*/
	void RenderDeferred::setWindowSize(float wWidth, float wHeight, const Camera &camera){
		_shader.bind();
		glUniform2f(_screenInv, 1.0f / wWidth, 1.0f / wHeight);
		glUniform1f(_near, camera.getNearPlane());
		glUniform1f(_far, camera.getFarPlane());
		float halfTanFowy = std::tan(camera.getFieldOfView() * 0.5f);
		glUniform1f(_top, halfTanFowy);
		glUniform1f(_right, halfTanFowy * camera.getAspectRatio());
	}
