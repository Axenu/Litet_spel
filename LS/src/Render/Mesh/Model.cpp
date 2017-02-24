#include "Render/Mesh/Model.h"

Model::Model() {

}


Model::Model(std::vector<ModelPart> &pMeshes, Skeleton *skeleton)
	: _meshParts(pMeshes), _skeleton(skeleton)
{
	initAABB();
}

Model::Model(ModelPart &part)
	: _meshParts(1)
{
	_meshParts[0] = part;
	initAABB();
}

void Model::setAnimController(AnimatedSkeleton *skeleton) {
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i].setSkeleton(skeleton);
}
Skeleton* Model::getSkeleton() {
	return _skeleton;
}

void Model::render(RenderInfo &fD, glm::mat4 &modelMatrix) const
{
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i].render(fD, modelMatrix);
}
void Model::transform(const glm::mat4 &mat) {
	for (unsigned int i = 0; i < _meshParts.size(); i++)
		_meshParts[i]._box = _meshParts[i].getMesh().getBox().transform(mat);
	initAABB();
}
const AABB& Model::getBox() const {
	return _aabb;
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

void Model::setAABB(const AABB & aabb)
{
	_aabb = aabb;
}

std::string Model::getName()
{
	return _name;
}



Model::~Model()
{

}

void Model::initAABB()
{
	if (_meshParts.size() > 0)
	{
		std::vector<glm::vec3> pos;
		for (unsigned int i = 0; i < _meshParts.size(); i++)
		{
			AABB tmpBB;
			tmpBB = _meshParts[i].getBox();
			pos.push_back(tmpBB.getMax());
			pos.push_back(tmpBB.getMin());
		}
		_aabb = AABB(&pos);
	}
}
