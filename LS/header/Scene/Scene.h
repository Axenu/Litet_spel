#pragma once
#include<vector>
#include"../GameObject.h"


class Scene
{
	std::vector<GameObject> _objects;
public:
	Scene();
	~Scene();
};

