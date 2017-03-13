#pragma once
//#define DEBUG

#include "Game/Scene/Scene.h"
#include "Event/EventManager.h"
#include "Scene/SceneEventManager.h"
#include "Render/Composition/RenderDeferred.h"
#include "Setting.h"
#include "camera.h"
#include "Level/ObjectFactory.h"
#include "Render/Mesh/Shader/ShadowCubeShader.h"
#include "Render/Mesh/Shader/SkinnedShadowCubeShader.h"

/* The game
*/
class Game
{
protected:
	/* Render vars:
	*/
	Setting _setting;
	GraphicsResource _resource;
	RenderDeferred _deferred;

	/* Scene vars:
	*/
	EventManager *_eventManager;
	std::unique_ptr<Scene> _scene;
	ObjectFactory _factory;
	std::unique_ptr<SceneEventManager> _sceneManager;


	ShadowCubeShader _shadowShader;
	SkinnedShadowCubeShader _skinnedShadowShader;

	virtual void compose(RenderInfo &rI);
	virtual std::unique_ptr<Scene> spawnScene() = 0;
public:
	Game(Setting &setting, EventManager *eventManager);
	virtual ~Game();
	void initiate();

	void update(float dT);
	void draw(float dT);

	Scene &getScene();
};
