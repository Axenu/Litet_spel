#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "MeshShader.h"


class DeferredMeshShader
	: public MeshShader
{
private:

	class DeferredMaterial :
		public MaterialLink {
	public:
		glm::vec4 _diffuse, _specular;
		float _shininess;

		DeferredMaterial(){}
		virtual ~DeferredMaterial(){}
	};

	GLint _mvpUniform, _mvUniform, _difColUniform, _specColUniform;
	void acquireUniforms();

public:
	DeferredMeshShader();
	virtual ~DeferredMeshShader();

	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *material) const;
	/* Link the shader to the material
	*/
	virtual std::shared_ptr<MaterialLink> linkMaterial(Material &mat) const;
};
