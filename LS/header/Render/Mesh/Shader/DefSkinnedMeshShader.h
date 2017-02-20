#pragma once
#include <glm/glm.hpp>
#include "MeshShader.h"


class DefSkinnedMeshShader
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

	GLint _vpUniform, _vUniform, _boneUniform, _difColUniform, _specColUniform;
	void acquireUniforms();

public:	
	DefSkinnedMeshShader();
	virtual ~DefSkinnedMeshShader();

	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *material) const;
	/* Link the shader to the material
	*/
	virtual std::shared_ptr<MaterialLink> linkMaterial(Material &mat) const;
};

