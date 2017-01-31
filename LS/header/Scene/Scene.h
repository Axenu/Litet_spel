#pragma once
#include<vector>
#include"../GameObject.h"
#include"Render/FrameData.h"
#include"node.h"

/* Scene holding the objects 
*/
class Scene
{
private:
	std::vector<GameObject> _objects;
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
	void add(GameObject &object);
	/* Fetches and culls the drawables for the scene
	*/
	void fetchObjects(FrameData &fD);
};

