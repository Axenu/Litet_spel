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

	std::vector<Model*> _models;
	std::vector<Mesh*> _mesh;
};