#include "Render/Mesh/ModelLoader.h"
#include "Render/Mesh/Animation/Skeleton.h"
#include "ModelConstruct.h"
#include "Animation/AnimationConstruct.h"

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
	ProcessAnimations(scene, construct);
	Model* model = new Model(construct._parts);

	//Assign modelName
	model->setName(modelName);
	_models.push_back(model);
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::string &modelName, ModelConstruct &construct)
{
	Mesh *outMesh;

	std::vector<const void*> vertexData;
	std::vector<gl::VertexAttribute> attri;
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
	attri.push_back(gl::VertexAttribute(0, GL_FLOAT, 3, sizeof(float))); //Pos attribute
	attri.push_back(gl::VertexAttribute(1, GL_FLOAT, 3, sizeof(float))); //Norm attribute
	vertexData.push_back(&pos[0]);	//Get position array start pointer
	vertexData.push_back(&norm[0]);		//Get normal array start pointer

	//Proess bones & weights
	std::vector<int> _bones;
	std::vector<glm::vec2> weights[MAX_BONEWEIGHTS];
	if (mesh->HasBones()) {
		//Init weight buffers
		for (int i = 0; i < 4; i++)
			weights[i] = std::vector<glm::vec2>(pos.size());
		//Get weights for each bone
		for (unsigned int bIndex = 0; bIndex < mesh->mNumBones; bIndex++) {
			aiBone *bone = mesh->mBones[bIndex];
			_bones.push_back(construct.getBoneIndex(bone->mName.C_Str()));

			//Fetch all bone weights
			for (unsigned int w = 0; w < bone->mNumWeights; w++) {
				aiVertexWeight weight = bone->mWeights[w];
				//Find free slot and insert
				for (int i = 0; i < MAX_BONEWEIGHTS; i++) {
					if (weights[i][weight.mVertexId].y <= 0) { //First slot without assigned weight
						weights[i][weight.mVertexId] = glm::vec2(bIndex, weight.mWeight);
						break;
					}
				}
			}
		}

		//Weight attris
		attri.push_back(gl::VertexAttribute(2, GL_FLOAT, 2, sizeof(float)));
		attri.push_back(gl::VertexAttribute(3, GL_FLOAT, 2, sizeof(float)));
		attri.push_back(gl::VertexAttribute(4, GL_FLOAT, 2, sizeof(float)));
		attri.push_back(gl::VertexAttribute(5, GL_FLOAT, 2, sizeof(float)));
		vertexData.push_back(&weights[0][0]);
		vertexData.push_back(&weights[1][0]);
		vertexData.push_back(&weights[2][0]);
		vertexData.push_back(&weights[3][0]);
	}
	gl::VAData va = gl::generateVAO_SoA(vertexData, attri, pos.size(), &indice[0], sizeof(indice[0]), indice.size()); // Create VAO
	outMesh = new Mesh(pos, indice, va);
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


AnimSize ReadAnimData(aiAnimation *anim, ModelConstruct& construct) {
	AnimSize size;
	size._numChannels = 0;
	size._numFloats = 0;
	size._numNodeChannels = 3;
	size._numBones = construct._bones.size();
	for (GLuint ch = 0; ch < anim->mNumChannels; ch++) {
		aiNodeAnim *channel = anim->mChannels[ch];
		int bone = construct.getBoneIndex(channel->mNodeName.C_Str());
		if (bone < 0)
			return size; //This channel does not animate a bone.
		size._numChannels++;
		size._numFloats += 3 * channel->mNumPositionKeys;
		size._numFloats += 3 * channel->mNumScalingKeys;
		size._numFloats += 4 * channel->mNumRotationKeys;
	}
	return size;
}
void ProcessChannel(aiNodeAnim *channel, ModelConstruct& construct, AnimationConstruct& anim) {

	int bone = construct.getBoneIndex(channel->mNodeName.C_Str());
	if (bone < 0)
		return; //This channel does not animate a bone.

	for (GLuint i = 0; i < channel->mNumPositionKeys; i++) {
		aiVectorKey key = channel->mPositionKeys[i];
		anim.insert(bone, 0, (float)key.mTime, glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z));
	}
	for (GLuint i = 0; i < channel->mNumScalingKeys; i++) {
		aiVectorKey key = channel->mScalingKeys[i];
		anim.insert(bone, 1, (float)key.mTime, glm::vec3(key.mValue.x, key.mValue.y, key.mValue.z));
	}
	for (GLuint i = 0; i < channel->mNumRotationKeys; i++) {
		aiQuatKey key = channel->mRotationKeys[i];
		anim.insert(bone, 2, (float)key.mTime, glm::quat(key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w));
	}
}

void ModelLoader::ProcessAnimations(const aiScene* scene, ModelConstruct& construct) {
	
	for (GLuint i = 0; i < scene->mNumAnimations; i++) {
		aiAnimation *anim = scene->mAnimations[i];
		AnimSize size = ReadAnimData(anim, construct);
		AnimationConstruct animConst(size);

		float duration = (float)(anim->mDuration * anim->mTicksPerSecond);
		for (GLuint ch = 0; ch < anim->mNumChannels; ch++) {
			aiNodeAnim *channel = anim->mChannels[ch];
			ProcessChannel(channel, construct, animConst);
		}
		construct._animations.push_back(animConst.generateAnim(anim->mName.C_Str(), duration));
	}
}