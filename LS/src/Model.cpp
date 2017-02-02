#include "Model.h"

Model::Model() {

	
}


Model::Model(std::vector<MeshPart> &pMeshes)
	: _meshParts(pMeshes)
{
}

Model::Model(MeshPart &part) 
	: _meshParts(1)
{
	_meshParts[0] = part;
}

void Model::render(RenderInfo &fD, glm::mat4 &modelMatrix) const
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i].render(fD, modelMatrix);
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