#include "Game/Level/Level.h"



Level::Level(const std::string &level, EventManager &events, MeshShader &shader)
	: _grid(level), _gridMesh(_grid.generateMesh())
{
	Material mat(&shader);
	mat.setColor("diffuse", glm::vec4(0.8f));
	mat.setColor("spec", glm::vec4(0.2f));
	mat.setFloat("shine", 20.f);
	ModelPart part(&_gridMesh, mat);
	Model m(part);
	setModel(m);
}


Level::~Level()
{
}

Grid& Level::getGrid() {
	return _grid;
}

void Level::onUpdate(float dT) {

}
