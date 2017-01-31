#include "Scene/Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}


/* Update all objects in the scene
*/
void Scene::update(float dT) {
	_root.update(dT);
}
/* Adds a game object to the scene.
*/
void Scene::add(GameObject &object) {
	object.setParent(&_root);
	_objects.push_back(std::move(object));
}
/* Fetches and culls the drawables for the scene
*/
void Scene::fetchObjects(FrameData &fD) {

}
