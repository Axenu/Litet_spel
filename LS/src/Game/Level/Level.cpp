#include "Game/Level/Level.h"



Level::Level(const std::string &level, EventManager &events, MeshShader &shader)
	: _grid(level, _lootPlace)
{
	_grid.generateMesh(_gridMesh);

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

void Level::getLight(PointLight* light)
{
	_light.push_back(light);
}

float Level::calcLightOnPosition(glm::vec3 playerPos)
{
	float wallDist = 0.0f;
	glm::vec4 posColor(0.0f);

	for (unsigned int i = 0; i < _light.size(); i++)
	{
		glm::vec3 lightRay = playerPos - _light[i]->_pos;

		if (glm::length(lightRay) < _light[i]->_fadeDist)
		{
			wallDist = _grid.getDist(_light[i]->_pos, lightRay, _light[i]->_fadeDist);

			if (wallDist > glm::length(lightRay) || wallDist == 0.0f)
			{
				lightRay = glm::normalize(lightRay);
				float diff = glm::max(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), lightRay), 0.0f);
				float distance = glm::length(lightRay);
				float att = glm::max(1.0f - (distance / _light[i]->_fadeDist), 0.0f);

				posColor += _light[i]->_diffuse * diff * att;
			}
		}
	}

	return (posColor.x + posColor.y + posColor.z + 0.6f);
}
