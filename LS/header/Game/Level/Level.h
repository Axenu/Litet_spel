#pragma once
#include "Game/Objects/StaticGameObject.h"
#include "Grid.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "Event/EventManager.h"
#include "Render/PointLight.h"

/* Level struct holding 'all' information about a specific level loaded for a game instance.
*/
class Level
	: public GameObject
{
private:

	Grid _grid;
	std::vector<PointLight*> _light;
	Mesh _gridMesh[3];

public:

	Level(const std::string &level, EventManager &events, MeshShader &shader);
	virtual ~Level();

	Grid& getGrid();

	virtual void onUpdate(float dT);

	void getLight(PointLight* light);

	float calcLightOnPosition(glm::vec3 playerPos);
};

