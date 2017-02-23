#pragma once
#include "Game/Objects/StaticGameObject.h"
#include "Grid.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "Event/EventManager.h"

/* Level struct holding 'all' information about a specific level loaded for a game instance.
*/
class Level
	: public GameObject
{
private:

	Grid _grid;
	Mesh _gridMesh;

public:

	Level(const std::string &level, EventManager &events, MeshShader &shader);
	virtual ~Level();

	Grid& getGrid();

	virtual void onUpdate(float dT);
};

