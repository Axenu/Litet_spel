#pragma once
#include "Render/Mesh/Shader/MeshShader.h"
#include "Render/Mesh/ModelPart.h"
#include "Render/Mesh/Animation/Bone.h"

struct ModelConstruct {
public:
	ModelConstruct(MeshShader *shader);
	MeshShader* _shader;
	std::vector<ModelPart> _parts;
	std::vector<Bone> _bones;

	/*	Get a bone index from string. -1 if nothing found */
	int getBoneIndex(const std::string &name);
};
