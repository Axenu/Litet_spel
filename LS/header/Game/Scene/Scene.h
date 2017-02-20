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

	Camera* _cam;

	QuadTreeNode _quadTree;
protected:
	/* The scene tree root.
	*/
	Node _root;
public:
	Scene();
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

	int loot(float pickDist);

	void initQuadTree(AABB &aabb);

	void getDynObjects(std::vector<GameObject*> &list, const glm::mat4 & mat);

};

