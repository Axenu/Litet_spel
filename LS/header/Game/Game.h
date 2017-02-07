#pragma once
#include"Game/Scene/Scene.h"
#include"Render/Composition/RenderDeferred.h"
#include "Event/EventManager.h"
#include"Setting.h"
#include"camera.h"

/* The game 
*/
class Game
{
protected:
	Setting _setting;
	EventManager &_event;
	Scene _scene;
	GraphicsResource _resource;
	RenderDeferred _deferred;
	Camera _camera;

	virtual void compose(RenderInfo &rI);
public:
	Game(Setting &setting, EventManager &events);
	virtual ~Game();
	virtual void initiate();

	void update(float dT);
	void draw();
};

