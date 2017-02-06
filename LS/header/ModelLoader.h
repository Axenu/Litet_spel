#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <gl/glInclude.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLoader
{
private:
	struct AssimpModel
	{
		std::string modelName;
		std::vector<glm::vec3> _position;
		std::vector<glm::vec3> _normal;
		std::vector<GLuint> _indices;
	};

	void LoadModel(std::string);
	AssimpModel ProcessMesh(aiMesh*, const aiScene*, std::string);
	void ProcessNode(aiNode*, const aiScene*, std::string);

	std::vector<AssimpModel*> _models;

public:
	ModelLoader();
	ModelLoader(const ModelLoader&);
	~ModelLoader();
	AssimpModel GetModel(std::string);

};