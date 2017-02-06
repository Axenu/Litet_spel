#include "Scene/Scene.h"



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

int Scene::loot(Camera & cam, int pickDist)
{
	float testDist;
	std::vector<int> indices;
	std::vector<float> distance;
	//Sorting out objects close enough to cam
	for (unsigned int i = 0; i < _objects.size(); i++)
	{
		testDist = cam.getDistance(*_objects[i]);
		if (testDist < pickDist)
		{
			indices.push_back(i);
			distance.push_back(testDist);
		}
	}
	//Sorting the lists with closest to camera first
	int swapIndice;
	for (unsigned int i = 0; i < distance.size(); i++)
	{
		testDist = distance[i];
		swapIndice = i;
		for (unsigned int j = (i + 1); j < distance.size(); j++)
		{
			if (distance[j] < testDist)
			{
				swapIndice = j;
			}
		}
		if (swapIndice != i)
		{
			std::iter_swap(indices.begin() + i, indices.begin() + swapIndice);
			std::iter_swap(distance.begin() + i, distance.begin() + swapIndice);
		}
	}
	//Pick against the lootobjects in range, from closest to furthest
	bool picked = false;
	int value = 0;
	for (unsigned int i = 0; i < indices.size(); i++)
	{
		picked = _objects[indices[i]]->pick(cam);
		if (picked)
		{
			LootObject* tmpObj = dynamic_cast<LootObject*> (_objects[indices[i]]);
			if (tmpObj == nullptr)
			{
				break;
			}
			else
			{
				value = tmpObj->getValue();
				break;
			}
		}
	}

	return value;
}
