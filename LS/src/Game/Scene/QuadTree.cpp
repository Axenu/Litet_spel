#include "Game/Scene/QuadTree.h"


QuadTreeNode::QuadTreeNode()
	:_aabb()
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_children[2] = nullptr;
	_children[3] = nullptr;
}

QuadTreeNode::QuadTreeNode(int depth)
	:_aabb()
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_children[2] = nullptr;
	_children[3] = nullptr;
	CreateNodes(depth);
}

QuadTreeNode::QuadTreeNode(const AABB &aabb, int depth)
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_children[2] = nullptr;
	_children[3] = nullptr;
	_aabb = aabb;
	CreateNodes(depth);
}


QuadTreeNode::~QuadTreeNode()
{
	deleteTree();
}

void QuadTreeNode::CreateNodes()
{
	deleteTree();
	CreateNodes(0);
}


void QuadTreeNode::AddObjects(GameObject * gameObject)
{
	std::vector<GameObject*> tmpList;
	tmpList.push_back(gameObject);
	AddObjects(tmpList);
}

void QuadTreeNode::removeObjectData(GameObject * data)
{
	auto iter = std::find(_objectData.begin(), _objectData.end(), data);
	if (iter != _objectData.end())
	{
		_objectData.erase(iter);
	}
}

void QuadTreeNode::CreateNodes(int depth)
{
	if (depth >= _maxDepth)
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
			_children[i]->CreateNodes(depth + 1);
		}
	}
}

void QuadTreeNode::AddObjects(std::vector<GameObject*>& data)
{
	if (_children[0] == nullptr)
	{
		for (unsigned int i = 0; i < data.size(); i++)
		{
			_objectData.push_back(data[i]);
		}
	}
	else
	{
		std::vector<GameObject*> childList[4];

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

		for (unsigned int i = 0; i < data.size(); i++)
		{
			//First check if the object is in any of the planes.
			glm::vec4 objectPos = data[i]->getWorldPos();
			if (std::abs(plane1.distance + objectPos.x) < 0.00001f)
			{
				_objectData.push_back(data[i]);
			}
			else if (std::abs(plane2.distance + objectPos.z) < 0.00001f)
			{
				_objectData.push_back(data[i]);
			}
			else
			{
				AABB tmpAABB = data[i]->getAABB();
				boxStatus1 = BBPlaneTest(tmpAABB, plane1);

				boxStatus2 = BBPlaneTest(tmpAABB, plane2);

				if (boxStatus1 == PlaneResult::Outside)//Right half
				{
					if (boxStatus2 == PlaneResult::Outside) //Lower
					{
						childList[2].push_back(data[i]);
					}
					else if (boxStatus2 == PlaneResult::Inside) //Upper
					{
						childList[3].push_back(data[i]);
					}
					else//Intersecting
					{
						_objectData.push_back(data[i]);
					}
				}
				else if (boxStatus1 == PlaneResult::Inside)//Left half
				{
					if (boxStatus2 == PlaneResult::Outside)//Lower
					{
						childList[1].push_back(data[i]);
					}
					else if (boxStatus2 == PlaneResult::Inside)//Upper
					{
						childList[0].push_back(data[i]);
					}
					else//Intersecting
					{
						_objectData.push_back(data[i]);
					}
				}
				else//Intersecting
				{
					_objectData.push_back(data[i]);
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

void QuadTreeNode::QuadTreeTest(std::vector<GameObject*>& gameObjects, const glm::mat4 & mat)
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

	TraverseTree(gameObjects, planes, 6);
}

void QuadTreeNode::removeObject(GameObject * data)
{
	if (_children[0] == nullptr)
	{
		//Remove object pointer
		removeObjectData(data);
	}
	else
	{
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

		//First check if the object is in any of the planes.
		glm::vec4 objectPos = data->getWorldPos();
		if (std::abs(plane1.distance + objectPos.x) < 0.00001f)
		{
			removeObjectData(data);
		}
		else if (std::abs(plane2.distance + objectPos.z) < 0.00001f)
		{
			removeObjectData(data);
		}
		else
		{
			AABB tmpAABB = data->getAABB();
			boxStatus1 = BBPlaneTest(tmpAABB, plane1);

			boxStatus2 = BBPlaneTest(tmpAABB, plane2);

			if (boxStatus1 == PlaneResult::Outside)//Right half
			{
				if (boxStatus2 == PlaneResult::Outside) //Lower
				{
					_children[2]->removeObject(data);
				}
				else if (boxStatus2 == PlaneResult::Inside) //Upper
				{
					_children[3]->removeObject(data);
				}
				else//Intersecting
				{
					//Remove object pointer
					removeObjectData(data);
				}
			}
			else if (boxStatus1 == PlaneResult::Inside)//Left half
			{
				if (boxStatus2 == PlaneResult::Outside)//Lower
				{
					_children[1]->removeObject(data);
				}
				else if (boxStatus2 == PlaneResult::Inside)//Upper
				{
					_children[0]->removeObject(data);
				}
				else//Intersecting
				{
					//Remove object pointer
					removeObjectData(data);
				}
			}
			else//Intersecting
			{
				//Remove object pointer
				removeObjectData(data);
			}
		}
	}
}

void QuadTreeNode::SetAABB(const AABB & aabb)
{
	_aabb = aabb;
}

void QuadTreeNode::TraverseTree(std::vector<GameObject*>& gameObjects, Plane * planes, const int &nrOfPlanes)
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
	for (unsigned int i = 0; i < _objectData.size(); i++)
	{
		gameObjects.push_back(_objectData[i]);
	}
	if (_children[0] != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			_children[i]->TraverseTree(gameObjects, planes, nrOfPlanes);
		}
	}

}

void QuadTreeNode::QuadTreeTest(std::vector<GameObject*>& gameObjects, AABB &aabb)
{
	if (AABBvAABB(_aabb, aabb))
	{
		for (unsigned int i = 0; i < _objectData.size(); i++)
		{
			//test if collide with aabb
			if (AABBvAABB(_objectData[i]->getAABB(), aabb))
			{
				gameObjects.push_back(_objectData[i]);
			}
		}
		if (_children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				_children[i]->QuadTreeTest(gameObjects, aabb);
			}
		}
	}
}

void QuadTreeNode::QuadTreeTest(std::vector<GameObject*>& gameObjects, const AABB &aabb, bool *compFunc(GameObject *obj))
{
	if (AABBvAABB(_aabb, aabb))
	{
		for (unsigned int i = 0; i < _objectData.size(); i++)
			//Check children intersect and it's searched for
			if (compFunc(_objectData[i]) && AABBvAABB(aabb, _objectData[i]->getAABB()))
				gameObjects.push_back(_objectData[i]);
		if (_children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				_children[i]->QuadTreeTest(gameObjects, aabb, compFunc);
			}
		}
	}
}

void QuadTreeNode::QuadTreeTest(std::vector<GameObject*>& gameObjects, glm::vec3 & dir, glm::vec3 & origin)
{
	QuadTreeTest(gameObjects, dir, origin, FLT_MAX);
}

void QuadTreeNode::QuadTreeTest(std::vector<GameObject*>& gameObjects, glm::vec3 & dir, glm::vec3 & origin, float distance)
{
	if (AABBIntersectTo(_aabb, dir, origin, distance))
	{
		for (unsigned int i = 0; i < _objectData.size(); i++)
		{
			gameObjects.push_back(_objectData[i]);
		}
		if (_children[0] != nullptr)
		{
			for (int i = 0; i < 4; i++)
			{
				_children[i]->QuadTreeTest(gameObjects, dir, origin);
			}
		}
	}
}

void QuadTreeNode::deleteTree()
{
	for (int i = 0; i < 4; i++)
	{
		if (_children[i] != nullptr)
		{
			delete _children[i];
		}
	}
}
