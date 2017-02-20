#pragma once

#include<string>
#include"../../Shader.h"
#include"../../RenderInfo.h"
#include "../ModelPart.h"
#include"../Material.h"
#include"../MaterialLink.h"

class MeshShader
{
protected:
	Shader _shader;

public:
	MeshShader(const std::string &shader);
	MeshShader(const std::string &vertShader, const std::string &fragShader);
	MeshShader(Shader &shader);
	virtual ~MeshShader();
	/* Bind shader and assign related uniforms
	*/
	virtual void assignUniforms(RenderInfo &fD, const ModelPart& part, const glm::mat4 &modelMatrix, MaterialLink *material) const = 0;
	/* Link the shader to the material
	*/
	virtual std::shared_ptr<MaterialLink> linkMaterial(Material &mat) const = 0;
};

