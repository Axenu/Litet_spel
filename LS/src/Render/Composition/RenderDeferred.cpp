#include "Render/Composition/RenderDeferred.h"
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
		_pLightPos = _shader.getUniform("pLightPos");
		_pLightDiffCol = _shader.getUniform("pLightDif");
		_pLightSpecCol = _shader.getUniform("pLightSpec");
		_pLightFade = _shader.getUniform("pLightFade");

		//Other
		_lightNade = _shader.getUniform("viewGrenadePosition");

		//Bind samplers
		if (!_shader.bindSampler("colBuffer", 0))	return false;
		if (!_shader.bindSampler("norBuffer", 1))	return false;
		if (!_shader.bindSampler("specBuffer", 2))	return false;
		if (!_shader.bindSampler("depthBuffer", 3))	return false;
		return true;
	}

	/* Assign frame uniforms
	*/
	void RenderDeferred::assignUniforms(RenderInfo &rI){
		std::vector<PointLight>& lights = rI._pLightInfo;
		//Clamp light count
		unsigned int numLights = lights.size() < MAXLIGHTCOUNT ? lights.size() : MAXLIGHTCOUNT;
		/* Prepare uniform data by arranging light data into arrays.
		*/
		glm::vec3 POS[MAXLIGHTCOUNT], DIF[MAXLIGHTCOUNT], SPEC[MAXLIGHTCOUNT];
		float FADE[MAXLIGHTCOUNT];
		for (unsigned int i = 0; i < numLights; i++) {
			//Transform positions in to viewspace
			POS[i] = rI._V * glm::vec4(lights[i]._pos, 1.0f);
			DIF[i] = lights[i]._diffuse;
			SPEC[i] = lights[i]._specular;
			FADE[i] = lights[i]._fadeDist;
		}
		/*	Bind resources
		*/
		rI._resource.getDeffered().bindTextures();
		//	Light uniforms
		glUniform1ui(_pNumLights, numLights);
		glUniform3fv(_pLightPos, numLights, (const GLfloat*)&POS[0]);
		glUniform3fv(_pLightDiffCol, numLights, (const GLfloat*)&DIF[0]);
		glUniform3fv(_pLightSpecCol, numLights, (const GLfloat*)&SPEC[0]);
		glUniform1fv(_pLightFade, numLights, (const GLfloat*)&FADE[0]);


		//Other
		glm::vec3 antiLightPos = rI._V *  glm::vec4(rI._lightGrenadePos, 1.f);
		glUniform3f(_lightNade, antiLightPos.x, antiLightPos.y, antiLightPos.z);
	}

	/* Call on window size change
	*/
	void RenderDeferred::setWindowSize(float wWidth, float wHeight, const Camera &camera){
		_shader.bind();
		glUniform2f(_screenInv, 1.0f / wWidth, 1.0f / wHeight);
		glUniform1f(_near, camera.getNearPlane());
		glUniform1f(_far, camera.getFarPlane());
		float halfTanFowy = (float)std::tan(camera.getFieldOfView() * 0.5);
		glUniform1f(_top, halfTanFowy);
		glUniform1f(_right, halfTanFowy * camera.getAspectRatio());
	}
