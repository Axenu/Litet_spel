#include "Model.h"

Model::Model() {

	
}


Model::Model(std::vector<Mesh*> pMeshes, std::vector<MeshShader*> pShaders, std::vector<Material*> pMaterials)
{
	for (unsigned int i = 0; i < pMeshes.size(); i++)
	{
		MeshPart tmpMP;
		tmpMP._mesh = pMeshes[i];
		tmpMP._shader = pShaders[i];
		tmpMP._material = pMaterials[i];
		_meshParts.push_back(tmpMP);
	}
}

Model::Model(Mesh * pMesh, MeshShader * pShader, Material * pMaterial)
{
	MeshPart tmpMP;
	tmpMP._mesh = pMesh;
	tmpMP._shader = pShader;
	tmpMP._material = pMaterial;
	_meshParts.push_back(tmpMP);
}

void Model::render(FrameData &fD, glm::mat4 &modelMatrix) const
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
	{
		_meshParts[i]._shader->assignUniforms(fD, modelMatrix);
		_meshParts[i]._mesh->render();
	}
}
const std::vector<MeshPart>& Model::getParts() const {
	return _meshParts;
}

bool Model::pick(glm::vec3 origin, glm::vec3 dir)
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
	{
		if (_meshParts[i]._mesh->pick(origin, dir))
		{
			return true;
		}
	}
	return false;
}



Model::~Model() 
{

}