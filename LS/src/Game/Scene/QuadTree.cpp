#include "Game/Scene/QuadTree.h"


QuadTreeNode::QuadTreeNode()
	:_aabb(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(22.0f, 2.0f, 30.0f))
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_children[2] = nullptr;
	_children[3] = nullptr;
}

QuadTreeNode::QuadTreeNode(int depth)
	:_aabb(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 2.0f, 22.0f))
{
	CreateNodes(depth);
}

QuadTreeNode::QuadTreeNode(int depth, const AABB &aabb)
{
	_aabb = aabb;
	CreateNodes(depth);
}


QuadTreeNode::~QuadTreeNode()
{
	for (int i = 0; i < 4; i++)
	{
		if (_children[i] != nullptr)
		{
			delete _children[i];
		}
	}
}

void QuadTreeNode::CreateNodes(int & maxDepth)
{
	CreateNodes(0, maxDepth);
}

void QuadTreeNode::AddObjects(std::vector<GameObject*>& data)
{
	std::map<int, GameObject*> tmpMap;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		tmpMap.insert(std::pair<int, GameObject*>(i, data[i]));
	}

	AddObjects(tmpMap);
}

void QuadTreeNode::AddObjects(unsigned int indice, GameObject * gameObject)
{
	std::map<int, GameObject*> tmpMap;
	tmpMap.insert(std::pair<int, GameObject*>(indice, gameObject));
	AddObjects(tmpMap);
}

void QuadTreeNode::CreateNodes(int depth, int & maxDepth)
{
	if (depth >= maxDepth)
	{
		for (int i = 0; i < 4; i++)
		{
			_children[i] = nullptr;
		}
	}
	else
	{
		glm::vec3 tmp;
		tmp.x = _aabb.getMax().x - _aabb.getMin().x;
		tmp.y = _aabb.getMax().y;
		tmp.z = _aabb.getMax().z - _aabb.getMin().z;
		AABB tmpAABB;
		glm::vec3 tmpMin;
		glm::vec3 tmpMax;

		//Calc bottomleft AABB
		_children[0] = new QuadTreeNode;
		tmpMin = _aabb.getMin();
		tmpMax.x = _aabb.getMin().x + tmp.x / 2;
		tmpMax.y = _aabb.getMax().y;
		tmpMax.z = _aabb.getMin().z + tmp.z / 2;

		//SetValues to child
		tmpAABB.setMin(tmpMin);
		tmpAABB.setMax(tmpMax);
		_children[0]->SetAABB(tmpAABB);

		//Calc upperLeft AABB
		_children[1] = new QuadTreeNode;
		tmpMin = _aabb.getMin();
		tmpMin.z = _aabb.getMin().z + tmp.z / 2;
		tmpMax = _aabb.getMax();
		tmpMax.x = _aabb.getMin().x + tmp.x / 2;

		//SetValues to child
		tmpAABB.setMin(tmpMin);
		tmpAABB.setMax(tmpMax);
		_children[1]->SetAABB(tmpAABB);

		//Calc upperRight AABB
		_children[2] = new QuadTreeNode;
		tmpMin.x = _aabb.getMin().x + tmp.x / 2;
		tmpMax.y = _aabb.getMax().y;
		tmpMin.z = _aabb.getMin().z + tmp.z / 2;
		tmpMax = _aabb.getMax();

		//SetValues to child
		tmpAABB.setMin(tmpMin);
		tmpAABB.setMax(tmpMax);
		_children[2]->SetAABB(tmpAABB);

		//Calc bottomRight AABB
		_children[3] = new QuadTreeNode;
		tmpMin = _aabb.getMin();
		tmpMin.x = _aabb.getMin().x + tmp.x / 2;
		tmpMax = _aabb.getMax();
		tmpMax.z = _aabb.getMin().z + tmp.z / 2;

		//SetValues to child
		tmpAABB.setMin(tmpMin);
		tmpAABB.setMax(tmpMax);
		_children[3]->SetAABB(tmpAABB);
		
		for (int i = 0; i < 4; i++)
		{
			_children[i]->CreateNodes(depth + 1, maxDepth);
		}
	}
}

void QuadTreeNode::AddObjects(std::map<int, GameObject*>& data)
{
	if (_children[0] == nullptr)
	{
		for (auto const& tmpData : data)
		{
			_indiceData.push_back(tmpData.first);
		}
	}
	else
	{
		std::map<int, GameObject*> childList[4];

		glm::vec3 tmp;
		tmp.x = _aabb.getMax().x - _aabb.getMin().x;
		tmp.y = _aabb.getMax().y;
		tmp.z = _aabb.getMax().z - _aabb.getMin().z;

		Plane plane1;
		plane1.distance = -(_aabb.getMin().x + tmp.x / 2);
		plane1.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		PlaneResult boxStatus1;

		Plane plane2; // X-plane
		plane2.distance = -(_aabb.getMin().z + tmp.z / 2);
		plane2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		PlaneResult boxStatus2;

		for (auto const& tmpData : data)
		{
			AABB tmpAABB = tmpData.second->getAABB();
			boxStatus1 = BBPlaneTest(tmpAABB, plane1);

			boxStatus2 = BBPlaneTest(tmpAABB, plane2);

			if (boxStatus1 == PlaneResult::Outside)//Right half
			{
				if (boxStatus2 == PlaneResult::Outside) //Upper
				{
					childList[2].insert(tmpData);
				}
				else if (boxStatus2 == 1) //Lower
				{
					childList[3].insert(tmpData);
				}
				else//Intersecting
				{
					childList[2].insert(tmpData);
					childList[3].insert(tmpData);
				}
			}
			else if (boxStatus1 == PlaneResult::Inside)//Left half
			{
				if (boxStatus2 == PlaneResult::Outside)//Upper
				{
					childList[1].insert(tmpData);
				}
				else if (boxStatus2 == PlaneResult::Inside)//Lower
				{
					childList[0].insert(tmpData);
				}
				else//Intersecting
				{
					childList[0].insert(tmpData);
					childList[1].insert(tmpData);
				}
			}
			else//Intersecting
			{
				if (boxStatus2 == PlaneResult::Outside)//Upper
				{
					childList[1].insert(tmpData);
					childList[2].insert(tmpData);
				}
				else if (boxStatus2 == PlaneResult::Inside)//Lower
				{
					childList[0].insert(tmpData);
					childList[3].insert(tmpData);
				}
				else//Intersecting
				{
					childList[0].insert(tmpData);
					childList[1].insert(tmpData);
					childList[2].insert(tmpData);
					childList[3].insert(tmpData);
				}
			}

		}
		for (int i = 0; i < 4; i++)
		{
			if (childList[i].size() > 0)
			{
				_children[i]->AddObjects(childList[i]);
			}
		}
	}
}

void QuadTreeNode::QuadTreeTest(std::vector<int>& indices, const glm::mat4 & mat)
{
	Plane planes[6];
	
	//Left
	planes[0].normal.x = -(mat[0][3] + mat[0][0]);
	planes[0].normal.y = -(mat[1][3] + mat[1][0]);
	planes[0].normal.z = -(mat[2][3] + mat[2][0]);
	planes[0].distance = -(mat[3][3] + mat[3][0]);

	//Right
	planes[1].normal.x = -(mat[0][3] - mat[0][0]);
	planes[1].normal.y = -(mat[1][3] - mat[1][0]);
	planes[1].normal.z = -(mat[2][3] - mat[2][0]);
	planes[1].distance = -(mat[3][3] - mat[3][0]);

	//Top
	planes[2].normal.x = -(mat[0][3] - mat[0][1]);
	planes[2].normal.y = -(mat[1][3] - mat[1][1]);
	planes[2].normal.z = -(mat[2][3] - mat[2][1]);
	planes[2].distance = -(mat[3][3] - mat[3][1]);

	//Bottom
	planes[3].normal.x = -(mat[0][3] + mat[0][1]);
	planes[3].normal.y = -(mat[1][3] + mat[1][1]);
	planes[3].normal.z = -(mat[2][3] + mat[2][1]);
	planes[3].distance = -(mat[3][3] + mat[3][1]);

	//Near
	planes[4].normal.x = -(mat[0][3] + mat[0][2]);
	planes[4].normal.y = -(mat[1][3] + mat[1][2]);
	planes[4].normal.z = -(mat[2][3] + mat[2][2]);
	planes[4].distance = -(mat[3][3] + mat[3][2]);

	//Far
	planes[5].normal.x = -(mat[0][3] - mat[0][2]);
	planes[5].normal.y = -(mat[1][3] - mat[1][2]);
	planes[5].normal.z = -(mat[2][3] - mat[2][2]);
	planes[5].distance = -(mat[3][3] - mat[3][2]);

	for (int i = 0; i < 6; i++)
	{
		float denom = 1.0f / glm::length(planes[i].normal);
		planes[i].normal.x *= denom;
		planes[i].normal.y *= denom;
		planes[i].normal.z *= denom;
		planes[i].distance *= denom;
	}

	TraverseTree(indices, planes, 6);
}

void QuadTreeNode::SetAABB(const AABB & aabb)
{
	_aabb = aabb;
}

void QuadTreeNode::TraverseTree(std::vector<int>& indices, Plane * planes, const int &nrOfPlanes)
{
	PlaneResult testResult;
	for (int i = 0; i < nrOfPlanes; i++)
	{
		testResult = BBPlaneTest(_aabb, planes[i]);
		if (testResult == PlaneResult::Outside)
		{
			return;
		}
	}
	if (_children[0] == nullptr)
	{
		for (int i = 0; i < _indiceData.size(); i++)
		{
			indices.push_back(_indiceData[i]);
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			_children[i]->TraverseTree(indices, planes, nrOfPlanes);
		}
	}
}

