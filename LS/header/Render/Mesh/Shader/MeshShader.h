#pragma once

#include<string>
#include"../../Shader.h"
#include"../../RenderInfo.h"
#include"../Material.h"
#include"../MaterialLink.h"

class MeshShader
{
protected:
	Shader _shader;

public:
	MeshShader(const std::string &shader);
	MeshShader(Shader &shader);
	virtual ~MeshShader();
	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const glm::mat4 &modelMatrix, MaterialLink *material) const = 0;
	/* Link the shader to the material
	*/
	virtual std::shared_ptr<MaterialLink> linkMaterial(Material &mat) const = 0;
};

