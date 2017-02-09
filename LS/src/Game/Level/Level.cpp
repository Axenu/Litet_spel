#include "Game/Level/Level.h"



Level::Level(const std::string &level, EventManager &events, MeshShader &shader)
	: _grid(level), _gridMesh(_grid.generateMesh())
{
	Material mat(&shader);
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
