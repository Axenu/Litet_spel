#pragma once
#include<vector>
#include"Game/Objects/GameObject.h"
#include"Game/Objects/LootObject.h"
#include"DrawFrame.h"
#include"node.h"
#include "Game/camera.h"
#include "Game/Scene/QuadTree.h"
#include "Game/Objects/PointLightObject.h"

/* Scene holding the objects
*/
class Scene
{
private:
	/* Data ownership of objects in the scene.
	*/
	std::vector<GameObject*> _staticObjects;
	std::vector<GameObject*> _dynamicObjects;
	/* Scene node objects.
	*/
	std::vector<Node*> _nodes;
	/* Reference to the current sccene camera */
	Camera* _cam;
	/* Culling tree for static objects */
	QuadTreeNode _quadTree;

	void initQuadTree(AABB &aabb);
protected:
	GameObject* _rootObject;
	/* The scene tree root.
	*/
	std::unique_ptr<Node> _root;
public:
	Scene(std::unique_ptr<GameObject> &root, AABB sceneBounds);
	~Scene();

	Camera& setCamera(Setting &camSetting);
	Camera& getCamera();

	/* Update all objects in the scene
	*/
	void update(float dT);
	/* Adds a game object to the scene.
	*/
	void add(GameObject *object, bool dynamic);

	/* Add a node object to the scene
	*/
	void addNode(Node *object);
	/* Remove the object returns the pointer to any leftover data
	*/
	GameObject* remove(GameObject *object, bool deleteObj = true);
	/* Remove the node object returns the pointer to any leftover data
	*/
	Node* removeNode(Node *object, bool deleteObj = true);
	/* Populates the DrawFrame by fetching and culling the scene drawables
	*/
	void fetchDrawables(DrawFrame &dF);

	void fetchDrawables(DrawFrame &dF, AABB &aabb);

	/* Get a list of objects inside the bounds and of the specific type. */
	template<type::GameObjectType Type>
	std::vector<GameObject*> fetchObjects(const AABB& aabb);
	/* Fetch a set of game objects compared with the function */
	std::vector<GameObject*> fetchObjects(const AABB& aabb, bool *(compFunc)(GameObject* obj));
	int loot(float pickDist);

	void getDynObjects(std::vector<GameObject*> &list, const glm::mat4 & mat);

};


#pragma region Template fetch funcs

template<type::GameObjectType Type>
bool isGameType(GameObject *obj)
{
	return obj->_type == Type;
}

template<type::GameObjectType Type>
std::vector<GameObject*> Scene::fetchObjects(const AABB& aabb)
{
	return fetchObjects(aabb, &isGameType<Type>)
}
#pragma endregion