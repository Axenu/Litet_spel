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
	virtual ~QuadTreeNode();
	void CreateNodes(int &maxDepth);
	void AddObjects(std::vector<GameObject*> &data);
	void AddObjects(unsigned int indice, GameObject* gameObject);
	void QuadTreeTest(std::vector<int> &indices, const glm::mat4 &mat);

private:
	glm::vec3 _BBMin;
	glm::vec3 _BBMax;
	std::vector<int> _indiceData;
	QuadTreeNode* _children[4];
	void CreateNodes(int depth, int &maxDepth);
	void AddObjects(std::map<int, GameObject*> &data);
	void TraverseTree(std::vector<int> &indices, Plane *planes, const int &nrOfPlanes);
};