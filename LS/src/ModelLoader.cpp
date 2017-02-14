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

#pragma region AssimpConversion methods

/* Converts assimp 4x4 matrix to glm::mat4x4
*/
glm::mat4 convertAssimpMatrix(aiMatrix4x4 matrix) {
	glm::mat4 m;
	m[0] = glm::vec4(matrix.a1, matrix.b1, matrix.c1, matrix.d1);
	m[1] = glm::vec4(matrix.a2, matrix.b2, matrix.c2, matrix.d2);
	m[2] = glm::vec4(matrix.a3, matrix.b3, matrix.c3, matrix.d3);
	m[3] = glm::vec4(matrix.a4, matrix.b4, matrix.c4, matrix.d4);
	return m;
}

#pragma endregion

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
	ProcessBones(scene->mRootNode, scene, construct); //Find bones
	ProcessNode(scene->mRootNode, scene, modelName, construct);
	Model* model = new Model(construct._parts);

	//Assign modelName
	model->setName(modelName);
	_models.push_back(model);
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
		std::vector<int> _bones;
		for (unsigned int i = 0; i < mesh->mNumBones; i++) {
			aiBone *bone = mesh->mBones[i];
			_bones.push_back(construct.getBoneIndex(bone->mName.C_Str()));
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

	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, modelName, construct);
	}
	//Process children first gen
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, modelName, construct);
	}
}

int push_bone(aiNode *node, aiNode* root, ModelConstruct &construct) {
	if (node == root || node == root->mParent) 
		return -1;
	//Find if bone is generated
	int ind = construct.getBoneIndex(node->mName.C_Str());
	if (ind == -1) {
		//Not found generate parent as bone
		ind = push_bone(node->mParent, root, construct);
		construct._bones.push_back(Bone(node->mName.C_Str(), glm::mat4(), -1, ind));
		//Return bone index
		return construct._bones.size() - 1;
	}
	//Return bone index
	return ind;
}
void ModelLoader::ProcessBones(aiNode* node, const aiScene* scene, ModelConstruct& construct) {

	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		for (GLuint b = 0; b < mesh->mNumBones; b++)
		{
			aiBone *bone = mesh->mBones[b];
			aiNode *boneNode = scene->mRootNode->FindNode(bone->mName);
			int index = push_bone(boneNode, node, construct);
			construct._bones[index]._invBindPose = convertAssimpMatrix(bone->mOffsetMatrix);
		}
	}
	

	//Process children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessBones(node->mChildren[i], scene, construct);
	}
}