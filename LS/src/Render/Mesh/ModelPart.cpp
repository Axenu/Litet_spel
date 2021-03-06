#include"Render/Mesh/ModelPart.h"

ModelPart::ModelPart()
	: _mesh(nullptr), _skeleton(nullptr), _material(nullptr), _box()
{
}

ModelPart::ModelPart(Mesh *mesh, Material &mat)
	: _mesh(mesh), _skeleton(nullptr), _material(mat), _box(mesh->getBox())
{
}

ModelPart::~ModelPart()
{
}

void ModelPart::render(RenderInfo &fD, const glm::mat4 &modelMatrix) const
{
	_material.assignUniforms(fD, *this, modelMatrix);
	_mesh->render();
}

void ModelPart::renderMeshOnly() const
{
	_mesh->render();
}

const Mesh& ModelPart::getMesh() const
{
	return *_mesh;
}
void ModelPart::setMaterial(Material & mat)
{
	_material = mat;
}
void ModelPart::setSkeleton(AnimatedSkeleton *skel)
{
	_skeleton = skel;
}
const AnimatedSkeleton* ModelPart::getSkeleton() const
{
	return _skeleton;
}
bool ModelPart::hasSkeleton() const
{
	return _skeleton != nullptr;
}
const AABB & ModelPart::getBox()
{
	return _box;
}
void ModelPart::setHasShadows(bool shadow)
{
	_hasShadow = shadow;
}
bool ModelPart::getHasShadow() const
{
	return _hasShadow;
}
