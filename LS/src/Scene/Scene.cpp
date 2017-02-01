#include "Scene/Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
	for (unsigned int i = 0; i < _objects.size(); i++)
		delete _objects[i];
}


/* Update all objects in the scene
*/
void Scene::update(float dT) {
	_root.update(dT);
}
/* Adds a game object to the scene.
*/
void Scene::add(GameObject *object) {
	//If object has no parent set the scene as the root.?
	if(!object->getParent())
		object->setParent(&_root);
	_objects.push_back(std::move(object));
}


void Scene::fetchDrawables(DrawFrame &dF) {
	for (unsigned int i = 0; i < _objects.size(); i++) {
		//Todo: Culling functionality
		dF.add(_objects[i]->getModel(), _objects[i]->getModelMatrix());
	}
}
