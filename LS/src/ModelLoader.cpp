#include "../header/ModelLoader.h"

ModelLoader::ModelLoader()
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile("cube.obj", aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene, "cube.obj");
}

ModelLoader::ModelLoader(const ModelLoader& other)
{

}

ModelLoader::~ModelLoader()
{

}

ModelLoader::AssimpModel ModelLoader::GetModel(std::string modelName)
{
	for (unsigned int i = 0; i < _models.size(); i++)
	{
		if (_models[i]->modelName == modelName)
		{
			return *_models[i];
		}
	}

	LoadModel(modelName);

	return *_models.back();
}

void ModelLoader::LoadModel(std::string modelName)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelName, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene, modelName);
}

ModelLoader::AssimpModel ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string modelName)
{
	AssimpModel assimpModel;

	//Get name of model
	assimpModel.modelName = modelName;

	//Process vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		assimpModel._position.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		assimpModel._normal.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
	}

	//Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			assimpModel._indices.push_back(face.mIndices[j]);
		}
	}

	return assimpModel;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::string modelName)
{
	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_models.push_back(&ProcessMesh(mesh, scene, modelName));
	}

	//Do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, modelName);
	}
}
