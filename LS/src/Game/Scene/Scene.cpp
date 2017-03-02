#include "Game/Scene/Scene.h"
#include <memory>


Scene::Scene(std::unique_ptr<GameObject>& root, AABB sceneBounds)
	: _quadTree(), _rootObject(root.get()), _root(root.release())
{
	_quadTree.SetAABB(sceneBounds);
	_quadTree.CreateNodes();
}

Scene::~Scene()
{
	for (unsigned int i = 0; i < _staticObjects.size(); i++)
		delete _staticObjects[i];
	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
		delete _dynamicObjects[i];
	for (unsigned int i = 0; i < _nodes.size(); i++)
		delete _nodes[i];
}


/* Update all objects in the scene
*/
void Scene::update(float dT) {
	_root->update(dT);
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
void Scene::add(GameObject *object, bool dynamic) {
	//If object has no parent set the scene as the root.?
	if (!dynamic)
	{
		if (!object->getParent())
			object->setParent(_root.get());
		//Initiates the modelMatrix
		object->update(0.0f);
		//adds the object to the QuadTree
		_quadTree.AddObjects(object);
		_staticObjects.push_back(std::move(object));
	}
	else
	{
		if (!object->getParent())
			object->setParent(_root.get());
		object->update(0.0f);
		_dynamicObjects.push_back(std::move(object));
	}

}

void Scene::addNode(Node *object) {
	if (!object->getParent())
		object->setParent(_root.get());
	_nodes.push_back(object);

}
GameObject* Scene::remove(GameObject *gameObj, bool deleteObj) {
	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
	{
		if (_dynamicObjects[i] == gameObj)
		{
			_dynamicObjects.erase(_dynamicObjects.begin() + i);
			gameObj->removeNode();
			if (deleteObj)
			{
				delete gameObj;
				return nullptr;
			}
			return gameObj;
		}
	}
	for (unsigned int i = 0; i < _staticObjects.size(); i++) {
		if (_staticObjects[i] == gameObj) {
			_staticObjects.erase(_staticObjects.begin() + i);
			_quadTree.removeObject(gameObj); //Clear object from the quadTree
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
	for (unsigned int i = 0; i < _staticObjects.size(); i++) {
		if (_staticObjects[i] == object) {
			_staticObjects.erase(_staticObjects.begin() + i);
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
	std::vector<GameObject*> drawList;
	getDynObjects(drawList, _cam->VPMatrix);
	_quadTree.QuadTreeTest(drawList, _cam->VPMatrix);
	drawList.push_back(_rootObject);
	for (unsigned int i = 0; i < drawList.size(); i++)
		drawList[i]->addToFrame(dF);
	/*for (unsigned int i = 0; i < _objects.size(); i++)
		_objects[i]->addToFrame(dF);*/
}

void Scene::fetchDrawables(DrawFrame &dF, AABB &aabb) {
	std::vector<GameObject*> drawList;
	getDynObjects(drawList, aabb);
	_quadTree.QuadTreeTest(drawList, aabb);
	drawList.push_back(_rootObject);
	for (unsigned int i = 0; i < drawList.size(); i++)
		drawList[i]->addToFrame(dF);
	/*for (unsigned int i = 0; i < _objects.size(); i++)
		_objects[i]->addToFrame(dF);*/
}


/* Fetch a set of game objects compared with the function */
std::vector<GameObject*> Scene::fetchStaticObjects(const AABB& aabb, bool *(compFunc)(GameObject* obj))
{
	std::vector<GameObject*> list;
	_quadTree.QuadTreeTest(list, aabb, compFunc);
	return list;
}


int Scene::loot(float pickDist)
{
	float testDist;
	std::vector<int> indices;
	std::vector<float> distance;
	std::vector<GameObject*> pickList;
	glm::vec3 lookat = _cam->getLookAt();
	glm::vec3 wpos = _cam->getWorldPos();
	_quadTree.QuadTreeTest(pickList, lookat, wpos, pickDist);

	//Sorting out objects close enough to cam
	for (unsigned int i = 0; i < pickList.size(); i++)
	{
		testDist = _cam->getDistance(*pickList[i]);
		if (testDist < pickDist)
		{
			indices.push_back(i);
			distance.push_back(testDist);
		}
	}
	//Sorting the lists with closest to camera first
	unsigned int swapIndice;
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
		if (pickList[indices[i]]->_type == type::GameObjectType::PointLight)
		{
			//If it is a pointlight dont regard it as a hit and continue with the next object in the list
			continue;
		}
		picked = pickList[indices[i]]->pick(*_cam);
		if (picked)
		{
			LootObject* tmpObj = dynamic_cast<LootObject*> (pickList[indices[i]]);
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

GameObject* Scene::pick(float pickDist)
{
	glm::vec3 lookAt = _cam->getLookAt();
	glm::vec3 wPos = _cam->getWorldPos();
	float minDist = pickDist;
	GameObject* retPtr = nullptr;
	//Search dynamic objects.
	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
	{
		PointLightObject* testPtr = dynamic_cast<PointLightObject*>(_dynamicObjects[i]);
		if (testPtr)
		{
			//Test to see if it is a pointLight.
			continue;
		}
		GameObject* ptr = _dynamicObjects[i];
		float dist;
		//If ray hits the object and is closer than the pick distance and the last hit object, set as new closest object.
		if (AABBIntersect(_dynamicObjects[i]->getAABB(), lookAt, wPos, dist) && dist < pickDist && dist < minDist)
		{
			retPtr = ptr;
			minDist = dist;
		}
	}
	//Search quadTree.
	std::vector<GameObject*> pickList;
	_quadTree.QuadTreeTest(pickList, lookAt, wPos, pickDist);
	for (unsigned int i = 0; i < pickList.size(); i++)
	{
		GameObject* ptr = pickList[i];
		float dist;
		//Check so it is not a doorway.
		if (ptr->_type != type::GameObjectType::Door)
		{
			//If ray hits the object and is closer than the pick distance and the last hit object, set as new closest object.
			if (AABBIntersect(pickList[i]->getAABB(), lookAt, wPos, dist) && dist < pickDist && dist < minDist)
			{
				retPtr = ptr;
				minDist = dist;
			}
		}

	}

	return retPtr;
}

bool Scene::takeOverGuard(float maxDist)
{
	return false;
}



void Scene::getDynObjects(std::vector<GameObject*> &list, const glm::mat4 & mat)
{
	//Create planes from projview matrix
	Plane planes[6];

	//Create a Frustum class/object:

	//Left
	planes[0].normal.x = -(mat[0][3] + mat[0][0]);
	planes[0].normal.y = -(mat[1][3] + mat[1][0]);
	planes[0].normal.z = -(mat[2][3] + mat[2][0]);
	planes[0].distance = -(mat[3][3] + mat[3][0]);

	//Right
	planes[1].normal.x = -(mat[0][3] - mat[0][0]);
	planes[1].normal.y = -(mat[1][3] - mat[1][0]);
	planes[1].normal.z = -(mat[2][3] - mat[2][0]);
	planes[1].distance = -(mat[3][3] - mat[3][0]);

	//Top
	planes[2].normal.x = -(mat[0][3] - mat[0][1]);
	planes[2].normal.y = -(mat[1][3] - mat[1][1]);
	planes[2].normal.z = -(mat[2][3] - mat[2][1]);
	planes[2].distance = -(mat[3][3] - mat[3][1]);

	//Bottom
	planes[3].normal.x = -(mat[0][3] + mat[0][1]);
	planes[3].normal.y = -(mat[1][3] + mat[1][1]);
	planes[3].normal.z = -(mat[2][3] + mat[2][1]);
	planes[3].distance = -(mat[3][3] + mat[3][1]);

	//Near
	planes[4].normal.x = -(mat[0][3] + mat[0][2]);
	planes[4].normal.y = -(mat[1][3] + mat[1][2]);
	planes[4].normal.z = -(mat[2][3] + mat[2][2]);
	planes[4].distance = -(mat[3][3] + mat[3][2]);

	//Far
	planes[5].normal.x = -(mat[0][3] - mat[0][2]);
	planes[5].normal.y = -(mat[1][3] - mat[1][2]);
	planes[5].normal.z = -(mat[2][3] - mat[2][2]);
	planes[5].distance = -(mat[3][3] - mat[3][2]);

	for (int i = 0; i < 6; i++)
	{
		float denom = 1.0f / glm::length(planes[i].normal);
		planes[i].normal.x *= denom;
		planes[i].normal.y *= denom;
		planes[i].normal.z *= denom;
		planes[i].distance *= denom;
	}

	//Test planes against Objects
	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
	{
		AABB aabb = _dynamicObjects[i]->getAABB();
		bool accepted = true;
		for (int j = 0; j < 6; j++)
		{
			if (BBPlaneTest(aabb, planes[j]) == PlaneResult::Outside)
			{
				accepted = false;
				break;
			}
		}
		if (accepted)
		{
			list.push_back(_dynamicObjects[i]);
		}
	}
}

void Scene::getDynObjects(std::vector<GameObject*> &list, AABB &iaabb)
{

	//test each aabb against light aabb
	for (unsigned int i = 0; i < _dynamicObjects.size(); i++)
	{
		AABB aabb = _dynamicObjects[i]->getAABB();
		if (AABBvAABB(aabb, iaabb))
		{
			list.push_back(_dynamicObjects[i]);
		}
	}
}
