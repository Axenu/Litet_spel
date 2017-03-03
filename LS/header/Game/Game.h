#pragma once
#define DEBUG

#include "Game/Scene/Scene.h"
#include "Render/Composition/RenderDeferred.h"
#include "Event/EventManager.h"
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
	Setting _setting;
	EventManager *_eventManager;
	std::unique_ptr<Scene> _scene;
	GraphicsResource _resource;
	RenderDeferred _deferred;
	ObjectFactory _factory;

	ShadowCubeShader _shadowShader;
	SkinnedShadowCubeShader _skinnedShadowShader;

	virtual void compose(RenderInfo &rI);
	virtual void setupRI(RenderInfo &rI) {};
public:
	Game(Setting &setting, EventManager *eventManager);
	virtual ~Game();
	virtual void initiate();

	void update(float dT);
	void draw();

	Scene &getScene();
};
