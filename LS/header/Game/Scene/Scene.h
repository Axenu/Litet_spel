#pragma once
#include<vector>
#include"Game/Objects/GameObject.h"
#include"DrawFrame.h"
#include"node.h"

/* Scene holding the objects 
*/
class Scene
{
private:
	/* Data ownership of objects in the scene.
	*/
	std::vector<GameObject*> _objects;
	/* Scene node objects.
	*/
	std::vector<Node*> _nodes;
protected:
	/* The scene tree root.
	*/
	Node _root;
public:
	Scene();
	~Scene();

	/* Update all objects in the scene
	*/
	void update(float dT);
	/* Adds a game object to the scene.
	*/
	void add(GameObject *object);
	/* Add a node object to the scene
	*/
	void addNode(Node *object);
	/* Remove the object returns the pointer to any leftover data
	*/
	GameObject* remove(GameObject *object, bool deleteObj = true);
	/* Populates the DrawFrame by fetching and culling the scene drawables
	*/
	void fetchDrawables(DrawFrame &dF);

};

