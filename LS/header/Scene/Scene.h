#pragma once
#include<vector>
#include"../GameObject.h"
#include"DrawFrame.h"
#include"node.h"
#include"../PointLightObject.h"

/* Scene holding the objects 
*/
class Scene
{
private:
	/* Data ownership of objects in the scene.
	*/
	std::vector<GameObject*> _objects;
	std::vector<PointLightObject*> _pLights;
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
	void add(PointLightObject *object);
	/* Populates the DrawFrame by fetching and culling the scene drawables
	*/
	void fetchDrawables(DrawFrame &dF);

};

