#include "../header/ModelLoader.h"
#include "Render/Mesh/ModelConstruct.h"

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
}
Model ModelLoader::GetModel(std::string modelName, Material &material)
{
	Model tmpModel = GetModel(modelName, material.getpShader());
	tmpModel.setMaterial(material);
	return tmpModel;
}

Model ModelLoader::GetModel(std::string modelName, MeshShader * shader)
{
	for (unsigned int i = 0; i < _models.size(); i++)
	{
		if (_models[i]->getName() == modelName)
		{
			return *_models[i];
		}
	}

	LoadModel(modelName, shader);
	//Check model was loaded
	if (_models.size() > 0)
		return *_models.back();
	std::cout << "Model failed to load: " + modelName;
	return Model();
}

void ModelLoader::LoadModel(std::string &modelName, MeshShader *shader)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelName, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ModelConstruct construct(shader);
	ProcessNode(scene->mRootNode, scene, modelName, construct);
	Model* model = new Model(construct._parts);

	//Assign modelName
	model->setName(modelName);
	_models.push_back(model);
}

int push_bone(aiNode *node, aiNode* root, ModelConstruct &construct) {
	int ind = construct.getBoneIndex(node->mName.C_Str());
	if (ind == -1) {
		ind = push_bone(node->mParent, root, construct);
		construct._bones.push_back(Bone(node->mName, )
	}
	return ind;
}
void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string &modelName, ModelConstruct &construct)
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

	//Proess bones
	if (mesh->HasBones()) {
		for (unsigned int i = 0; i < mesh->mNumBones; i++) {
			aiNode *bone = 
			construct.getBoneIndex()
		}
	}

	outMesh = new Mesh(pos, norm, indice);
	_mesh.push_back(outMesh);

	//Get Models material
	Material mat(construct._shader);
	aiMaterial* tmpMat = scene->mMaterials[mesh->mMaterialIndex];

	//Get Diffuse color
	aiColor4D aiCol;
	tmpMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiCol);
	glm::vec4 col = glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a);
	mat.setColor("diffuse", col);

	//Get spec color
	tmpMat->Get(AI_MATKEY_COLOR_SPECULAR, aiCol);
	col = glm::vec4(aiCol.r, aiCol.g, aiCol.b, aiCol.a);
	mat.setColor("spec", col);

	//Get shine
	float shine;
	tmpMat->Get(AI_MATKEY_SHININESS, shine);
	mat.setFloat("shine", shine);

	construct._parts.push_back( ModelPart(outMesh, mat));
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::string &modelName, ModelConstruct& construct)
{
	//Process children first gen
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, modelName, construct);
	}

	int boneParent = construct.getBoneIndex(node->mParent->mName.C_Str());
	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, modelName, construct);
	}
}
