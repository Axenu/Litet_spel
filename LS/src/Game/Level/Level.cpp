#include "Game/Level/Level.h"



Level::Level(EventManager &events, DeferredMeshShader &shader)
	: _grid(), _gridMesh(_grid.generateMesh())
{
	//setModel()
}


Level::~Level()
{
}

Grid& Level::getGrid() {
	return _grid;
}

void Level::onUpdate(float dT) {

}
