#include "Game/Scene/SceneEventManager.h"



SceneEventManager::SceneEventManager(EventManager &eventManager, IObjectFactory &factory, Scene &scene)
	: _eventManager(eventManager), _factory(factory), _scene(scene)
{
	_eventManager.listen(this, &SceneEventManager::createAntiLightGrenade);
	_eventManager.listen(this, &SceneEventManager::removeFromScene);
}

void SceneEventManager::createAntiLightGrenade(const CreateLightGrenade& e)
{
	_factory.createLightGrenade(e._model, e._pos, e._dir);
}
void SceneEventManager::removeFromScene(const RemoveSceneObject& e)
{
	_removeList.push_back(e._obj);
}

/* Cleaning up at end of frame
*/
void SceneEventManager::endFrame()
{
	for (unsigned int i = 0; i < _removeList.size(); i++)
	{
		_scene.remove(_removeList[i], true);
	}
	_removeList.clear();
}
SceneEventManager::~SceneEventManager()
{
	_eventManager.unlisten(this, &SceneEventManager::createAntiLightGrenade);
	_eventManager.unlisten(this, &SceneEventManager::removeFromScene);
}
