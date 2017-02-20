#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <gl/glInclude.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Render/Mesh/Model.h"
#include "Render/Mesh/Mesh.h"
#include "Render/Mesh/Material.h"

/* Forward declare
*/
struct ModelConstruct;

/* Maximum number of weights per vertex
*/
const static int MAX_BONEWEIGHTS = 4;

class ModelLoader
{
public:
	ModelLoader();
	ModelLoader(const ModelLoader&);
	virtual ~ModelLoader();
	Model GetModel(std::string, Material &material);
	Model GetModel(std::string, MeshShader *shader);

private:

	void LoadModel(std::string&, MeshShader *shader);
	void ProcessMesh(aiMesh*, const aiScene*, std::string&, ModelConstruct& construct);
	void ProcessNode(aiNode*, const aiScene*, std::string&, ModelConstruct& construct);
	void ProcessBones(aiNode* node, const aiScene* scene, ModelConstruct& construct);
	void ProcessAnimations(const aiScene* scene, ModelConstruct& construct);

	std::vector<Model*> _models;
	std::vector<Mesh*> _mesh;
	std::vector<std::unique_ptr<Skeleton>> _skeletons;
};