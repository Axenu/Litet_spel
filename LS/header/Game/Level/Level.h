#pragma once
#include "Game/Objects/StaticGameObject.h"
#include "Grid.h"
#include "Render/Mesh/Shader/DeferredMeshShader.h"
#include "Event/EventManager.h"
#include "Game/Scene/Scene.h"

/* Forward declare
*/
class PointLightObject;
/* Level struct holding 'all' information about a specific level loaded for a game instance.
*/
class Level
	: public StaticGameObject
{
public:

private:

	Grid _grid;
	Mesh _gridMesh[3];
	Scene*  _scene;

public:

	Level(const std::string &level, EventManager *eventManager, MeshShader &shader);
	void setScene(Scene *scene);
	virtual ~Level();

	Grid& getGrid();

	/* Access the scene */
	const Scene& getScene();

};
