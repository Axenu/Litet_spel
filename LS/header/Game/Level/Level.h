#pragma once
#include "../Objects/GameObject.h"
#include "GridDataStructure.h"
#include "Event/EventManager.h"


class Level
	: public GameObject
{
private:

	Grid _grid;
	Mesh _gridMesh;

public:

	Level(EventManager &events);
	~Level();

	Grid& getGrid();

	virtual void onUpdate(float dT);
};

