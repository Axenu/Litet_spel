#pragma once
#include "Render/Mesh/Shader/MeshShader.h"
#include "Render/Mesh/ModelPart.h"
#include "Render/Mesh/Animation/Bone.h"
#include "Render/Mesh/Animation/Animation.h"
#include "Render/Mesh/Animation/Skeleton.h"

struct ModelConstruct {
public:
	ModelConstruct(MeshShader *shader);
	~ModelConstruct();
	MeshShader* _shader;
	std::vector<ModelPart> _parts;
	std::vector<Bone> _bones;
	std::vector<std::shared_ptr<Animation>> _animations;

	/*	Get a bone index from string. -1 if nothing found */
	int getBoneIndex(const std::string &name);
	/* Generate the skeleton from the gathered data. Null if no skeleton data is avaiable */
	Skeleton* getSkeleton();
};
