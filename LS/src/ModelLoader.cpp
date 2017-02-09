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

	std::vector<ModelPart> modelParts;
	ProcessNode(scene->mRootNode, scene, modelName, shader, modelParts);
	Model* model = new Model(modelParts);

	//Assign modelName
	model->setName(modelName);
	_models.push_back(model);
}

ModelPart ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string &modelName, MeshShader *shader)
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

	//Get Models material
	Material mat(shader);
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

	return ModelPart(outMesh, mat);
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, std::string &modelName, MeshShader *shader, std::vector<ModelPart> &modelParts)
{
	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		modelParts.push_back(ProcessMesh(mesh, scene, modelName, shader));
	}
	//Do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, modelName, shader, modelParts);
	}
}
