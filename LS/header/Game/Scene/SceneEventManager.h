#pragma once
#include "Scene.h"
#include "Game/Level/IObjectFactory.h"
#include "Event/EventManager.h"
#include "Event/SceneEvents/CreateEvents.h"
#include "Event/SceneEvents/SceneEvent.h"

class SceneEventManager
{
private:
	EventManager &_eventManager;
	IObjectFactory &_factory;
	Scene &_scene;

	std::vector<GameObject*> _removeList;

	void createAntiLightGrenade(const CreateLightGrenade& e);
	void removeFromScene(const RemoveSceneObject& e);

public:
	SceneEventManager(EventManager &eventManager, IObjectFactory &factory, Scene &scene);
	~SceneEventManager();
	/* Cleaning up at end of frame
	*/
	void endFrame();
};

