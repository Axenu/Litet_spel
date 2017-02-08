#include "Game/Level/Level.h"



Level::Level(EventManager &events, DeferredMeshShader &shader)
	: _grid(), _gridMesh(_grid.generateMesh())
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
