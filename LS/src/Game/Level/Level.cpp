#include "Game/Level/Level.h"



Level::Level(const std::string &level, EventManager *eventManager, MeshShader &shader)
	: _grid(level)
{
	_grid.generateMesh(_gridMesh, 0.5f, 2.f, 0.3f);

	std::vector<ModelPart> parts;

	Material floorMat(&shader);
	floorMat.setColor("diffuse", glm::vec4(0.1f));
	floorMat.setColor("spec", glm::vec4(0.8f, 0.2f, 0.05f, 1.0f));
	floorMat.setFloat("shine", 20.f);
	ModelPart floorPart(&_gridMesh[0], floorMat);
	parts.push_back(floorPart);

	Material roofMat(&shader);
	roofMat.setColor("diffuse", glm::vec4(1.f));
	roofMat.setColor("spec", glm::vec4(0.2f));
	roofMat.setFloat("shine", 20.f);
	ModelPart roofPart(&_gridMesh[1], roofMat);
	parts.push_back(roofPart);

	Material wallMat(&shader);
	wallMat.setColor("diffuse", glm::vec4(0.8f));
	wallMat.setColor("spec", glm::vec4(0.2f));
	wallMat.setFloat("shine", 20.f);
	ModelPart wallPart(&_gridMesh[2], wallMat);
	parts.push_back(wallPart);

	Model m(parts);
	setModel(m);
}


Level::~Level()
{
}


void Level::setScene(Scene *scene)
{
	_scene = scene;
}

const Scene& Level::getScene()
{
	return *_scene;
}

Grid& Level::getGrid() {
	return _grid;
}

void Level::onUpdate(float dT) {

}
