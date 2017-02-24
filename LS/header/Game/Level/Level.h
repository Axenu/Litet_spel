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
	: public GameObject
{
public:
	std::vector<glm::ivec2> _lootPlace;
private:

	Grid _grid;
	std::vector<PointLight*> _light;
	Mesh _gridMesh[3];
	Scene*  _scene;

public:

	Level(const std::string &level, EventManager &events, MeshShader &shader);
	void setScene(Scene *scene);
	virtual ~Level();

	Grid& getGrid();
	
	/* Access the scene */
	const Scene& getScene();

	virtual void onUpdate(float dT);

	void getLight(PointLight* light);

	float calcLightOnPosition(glm::vec3 playerPos);
};

