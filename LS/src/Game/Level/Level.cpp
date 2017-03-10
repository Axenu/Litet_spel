#include "Game/Level/Level.h"



Level::Level(const std::string &level, EventManager *eventManager, MeshShader &shader)
	: _grid(level)
{
	_grid.generateMesh(_gridMesh, 0.5f, 2.f, 0.3f);

	std::vector<ModelPart> parts;

	Material floorMat(&shader);
	floorMat.setColor("diffuse", glm::vec4(0.15f));
	floorMat.setColor("spec", glm::vec4(0.3f));
	floorMat.setFloat("shine", 5.f);

	Material wallMat(&shader);
	wallMat.setColor("diffuse", glm::vec4(0.2f, 0.01f, 0.005f, 1.0f));
	wallMat.setColor("spec", glm::vec4(0.5f));
	wallMat.setFloat("shine", 20.f);
	//Create meshes
	ModelPart floorPart(&_gridMesh[0], floorMat);
	parts.push_back(floorPart);
	ModelPart roofPart(&_gridMesh[1], floorMat);
	parts.push_back(roofPart);
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
