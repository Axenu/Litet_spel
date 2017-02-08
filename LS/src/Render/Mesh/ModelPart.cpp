#include"Render/Mesh/ModelPart.h"

ModelPart::ModelPart()
	: _mesh(nullptr), _material(nullptr), _box() {
}

ModelPart::ModelPart(Mesh *mesh, Material *mat)
	: _mesh(mesh), _material(mat), _box(mesh->getBox()) {
}

ModelPart::~ModelPart()
{

}


void ModelPart::render(RenderInfo &fD, const glm::mat4 &modelMatrix) const {
	_material->assignUniforms(fD, modelMatrix);
	_mesh->render();
}

Mesh& ModelPart::getMesh() {
	return *_mesh;
}