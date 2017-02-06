#include "Game/Scene/Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
	for (unsigned int i = 0; i < _objects.size(); i++)
	 	delete _objects[i];
	for (unsigned int i = 0; i < _nodes.size(); i++)
		delete _nodes[i];
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

void Scene::addNode(Node *object) {
	if (!object->getParent())
		object->setParent(&_root);
	_nodes.push_back(object);

}
GameObject* Scene::remove(GameObject *gameObj, bool deleteObj) {
	for (unsigned int i = 0; i < _objects.size(); i++) {
		if (_objects[i] == gameObj) {
			_objects.erase(_objects.begin() + i);
			gameObj->removeNode(); //Clear the node from the tree
			if (deleteObj) {
				delete gameObj;
				return nullptr;
			}
			return gameObj;
		}
	}
	return gameObj;
}

void Scene::fetchDrawables(DrawFrame &dF) {
	for (unsigned int i = 0; i < _objects.size(); i++)
		_objects[i]->addToFrame(dF);
}
