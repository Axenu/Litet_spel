#include"MeshPart.h"

MeshPart::MeshPart()
	: _mesh(nullptr), _material(nullptr) {
}

MeshPart::MeshPart(Mesh *mesh, Material *mat)
	: _mesh(mesh), _material(mat) {
}


void MeshPart::render(RenderInfo &fD, const glm::mat4 &modelMatrix) const {
	_material->assignUniforms(fD, modelMatrix);
	_mesh->render();
}