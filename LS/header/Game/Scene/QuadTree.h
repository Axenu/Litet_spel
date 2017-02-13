#pragma once
#include <vector>
#include <map>
#include "Game/Objects/GameObject.h"
#include "IntersectionFunctions.h"

class QuadTreeNode
{
public:
	QuadTreeNode();
	virtual ~QuadTreeNode();
	void CreateNodes(std::map<int, GameObject*> &data, int depth, int &maxDepth);
	void QuadTreeTest(std::vector<int> &indices, const glm::mat4 &mat);
private:
	glm::vec3 _BBMin;
	glm::vec3 _BBMax;
	std::vector<int> _indiceData;
	QuadTreeNode* _children[4];
	void TraverseTree(std::vector<int> &indices, Plane *planes, const int &nrOfPlanes);
};