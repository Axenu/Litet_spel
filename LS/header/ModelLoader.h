#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <gl/glInclude.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Render/MeshShader.h"
#include "Model.h"
#include "MeshPart.h"

class ModelLoader
{
private:
	void LoadModel(std::string, MeshShader *shader);
	MeshPart ProcessMesh(aiMesh*, const aiScene*, std::string, MeshShader *shader);
	void ProcessNode(aiNode*, const aiScene*, std::string, MeshShader *shader);

	std::vector<Model*> _models;
	std::vector<Mesh*> _mesh;
	std::vector<Material*> _material;

public:
	ModelLoader();
	ModelLoader(const ModelLoader&);
	virtual ~ModelLoader();
	Model* GetModel(std::string, MeshShader *shader);

};