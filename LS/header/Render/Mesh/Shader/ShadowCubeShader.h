#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "MeshShader.h"
#include "../../PointLight.h"
class ShadowCubeShader
	: public MeshShader
{
private:

	/* Model matrix */
	GLint _mmUniform;
	void acquireUniforms();
protected:
	GLint _shadowMatricesLocation, _lightPosLocation, _farPlaneLocation;
public:
	ShadowCubeShader();
	ShadowCubeShader(const std::string & vertShader);
	virtual ~ShadowCubeShader();

	/* Sets up the cube shader for rendering to a specific point light
	*/
	void setUp(const PointLight &pointL);

	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *material) const;
	/* Link the shader to the material
	*/
	virtual std::shared_ptr<MaterialLink> linkMaterial(Material &mat) const;
};
