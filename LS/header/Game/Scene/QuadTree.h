#pragma once
#include <vector>
#include <map>
#include "Game/Objects/GameObject.h"
#include "IntersectionFunctions.h"

class QuadTreeNode
{
public:
	QuadTreeNode();
	QuadTreeNode(int depth);
	QuadTreeNode(int depth, const AABB &aabb);
	virtual ~QuadTreeNode();
	void CreateNodes(int &maxDepth);
	void AddObjects(std::vector<GameObject*> &data);
	void AddObjects(GameObject* gameObject);
	void QuadTreeTest(std::vector<GameObject*> &gameObjects, const glm::mat4 &mat);
	void removeObject(GameObject* gameObject);
	void SetAABB(const AABB &aabb);

private:
	AABB _aabb;
	std::vector<GameObject*> _indiceData;
	QuadTreeNode* _children[4];
	void CreateNodes(int depth, int &maxDepth);
	void TraverseTree(std::vector<GameObject*> &gameObjects, Plane *planes, const int &nrOfPlanes);
};