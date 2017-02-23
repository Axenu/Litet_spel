#pragma once
#include <vector>
#include <map>
#include <math.h>
#include "Game/Objects/GameObject.h"
#include "math/IntersectionFunctions.h"

class QuadTreeNode
{
public:
	QuadTreeNode();
	QuadTreeNode(int depth);
	QuadTreeNode(const AABB &aabb, int depth);
	virtual ~QuadTreeNode();
	void CreateNodes();
	void AddObjects(std::vector<GameObject*> &data);
	void AddObjects(GameObject* gameObject);
	void QuadTreeTest(std::vector<GameObject*>& gameObjects, AABB &aabb);
	void QuadTreeTest(std::vector<GameObject*>& gameObjects, const AABB &aabb, bool *compFunc(GameObject *obj));
	void QuadTreeTest(std::vector<GameObject*> &gameObjects, const glm::mat4 &mat);
	void QuadTreeTest(std::vector<GameObject*> &gameObjects, glm::vec3 &dir, glm::vec3 &origin);
	void QuadTreeTest(std::vector<GameObject*> &gameObjects, glm::vec3 &dir, glm::vec3 &origin, float distance);
	void removeObject(GameObject* data);
	void SetAABB(const AABB &aabb);


private:
	static const int _maxDepth = 4;
	AABB _aabb;
	std::vector<GameObject*> _objectData;
	QuadTreeNode* _children[4];
	void removeObjectData(GameObject* data);
	void CreateNodes(int depth);
	void TraverseTree(std::vector<GameObject*> &gameObjects, Plane *planes, const int &nrOfPlanes);
	void deleteTree();
};
