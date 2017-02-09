#include "Render/Mesh/Model.h"

Model::Model() {

	
}


Model::Model(std::vector<ModelPart> &pMeshes)
	: _meshParts(pMeshes)
{
}

Model::Model(ModelPart &part) 
	: _meshParts(1)
{
	_meshParts[0] = part;
}

void Model::render(RenderInfo &fD, glm::mat4 &modelMatrix) const
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i].render(fD, modelMatrix);
}
void Model::transform(const glm::mat4 &mat) {
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i]._box = _meshParts[i].getMesh().getBox().transform(mat);
}
const AABB& Model::getBox() const {
	if (_meshParts.size() > 0)
		return _meshParts[0]._box;
	return AABB();
}
const std::vector<ModelPart>& Model::getParts() const {
	return _meshParts;
}

bool Model::pick(glm::vec3 origin, glm::vec3 dir)
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
	{
		if (_meshParts[i].getMesh().pick(origin, dir))
		{
			return true;
		}
	}
	return false;
}

void Model::setName(std::string & name)
{
	_name = name;
}

void Model::setMaterial(Material & mat)
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
	{
		_meshParts[i].setMaterial(mat);
	}
}

std::string Model::getName()
{
	return _name;
}



Model::~Model() 
{

}