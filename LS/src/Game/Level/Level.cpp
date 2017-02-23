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
	StaticGameObject::init();
}


Level::~Level()
{
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
			wallDist = _grid.getDist(_light[i]->_pos, lightRay, _light[i]->_fadeDist, playerPos, wall);

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

	return (posColor.x + posColor.y + posColor.z + 0.5f);
}
