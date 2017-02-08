#include "../header/ModelLoader.h"

ModelLoader::ModelLoader()
{
	
}

ModelLoader::ModelLoader(const ModelLoader& other)
{

}

ModelLoader::~ModelLoader()
{
	for (unsigned int i = 0; i < _models.size(); i++)
	{
		delete _models[i];
		_models[i] = nullptr;
	}
	for (unsigned int i = 0; i < _mesh.size(); i++)
	{
		delete _mesh[i];
		_mesh[i] = nullptr;
	}
	for (unsigned int i = 0; i < _material.size(); i++)
	{
		delete _material[i];
		_material[i] = nullptr;
	}
}

Model ModelLoader::GetModel(std::string modelName, Material* material)
{
	for (unsigned int i = 0; i < _models.size(); i++)
	{
		if (_models[i]->getName() == modelName)
		{
			return *_models[i];
		}
	}

	LoadModel(modelName, material);

	return *_models.back();
}

void ModelLoader::LoadModel(std::string modelName, Material* material)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelName, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene, modelName, material);
}

ModelPart ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string modelName, Material* material)
{
	Mesh *outMesh;

	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<GLuint> indice;

	//Process vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		pos.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		norm.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
	}

	//Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indice.push_back(face.mIndices[j]);
		}
	}

	outMesh = new Mesh(pos, norm, indice);
	_mesh.push_back(outMesh);
	_material.push_back(material);
	return ModelPart(outMesh, material);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::string modelName, Material* material)
{
	std::vector<ModelPart> meshParts;

	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshParts.push_back(ProcessMesh(mesh, scene, modelName, material));
	}

	Model* model = new Model(meshParts);
	_models.push_back(model);

	//Do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, modelName, material);
	}
}
