#include "Game/Scene/Scene.h"



Scene::Scene()
	:_quadTreeRoot(3)
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
Camera& Scene::setCamera(Setting &camSetting) {
	Camera* cam = new Camera(camSetting);
	_nodes.push_back(cam);
	std::swap(cam, _cam);
	//remove the old camera if it existed
	if (cam)
		removeNode(cam);
	return *_cam;
}
Camera& Scene::getCamera() {
	return *_cam;
}
/* Adds a game object to the scene.
*/
void Scene::add(GameObject *object) {
	//If object has no parent set the scene as the root.?
	if(!object->getParent())
		object->setParent(&_root);
	//adds the object to the QuadTree
	_quadTreeRoot.AddObjects(_objects.size(), object);
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
/* Remove the node object returns the pointer to any leftover data
*/
Node* Scene::removeNode(Node *object, bool deleteObj) {
	for (unsigned int i = 0; i < _objects.size(); i++) {
		if (_objects[i] == object) {
			_objects.erase(_objects.begin() + i);
			object->removeNode(); //Clear the node from the tree
			if (deleteObj) {
				delete object;
				return nullptr;
			}
			return object;
		}
	}
	return object;
}

void Scene::fetchDrawables(DrawFrame &dF) {
	//std::vector<int> drawIndices;
	//_quadTreeRoot.QuadTreeTest(drawIndices, _cam->getViewMatrix());
	//for (unsigned int i = 0; i < drawIndices.size(); i++)
	//	_objects[drawIndices[i]]->addToFrame(dF);
	for (unsigned int i = 0; i < _objects.size(); i++)
		_objects[i]->addToFrame(dF);
}


int Scene::loot(int pickDist)
{
	float testDist;
	std::vector<int> indices;
	std::vector<float> distance;
	//Sorting out objects close enough to cam
	for (unsigned int i = 0; i < _objects.size(); i++)
	{
		testDist = _cam->getDistance(*_objects[i]);
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
		picked = _objects[indices[i]]->pick(*_cam);
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
				this->remove(tmpObj, true);
				break;
			}
		}
	}

	return value;
}
