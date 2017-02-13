#include "Game/Scene/QuadTree.h"


QuadTreeNode::QuadTreeNode()
{

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

void QuadTreeNode::CreateNodes(std::map<int, GameObject*>& data, int depth, int & maxDepth)
{
	if (depth > maxDepth)
	{
		for (auto const& tmpData : data)
		{
			_indiceData.push_back(tmpData.first);
		}
		for (int i = 0; i < 4; i++)
		{
			_children[i] = nullptr;
		}
	}
	else
	{
		std::map<int, GameObject*> childList[4];

		glm::vec3 tmp;
		tmp.x = _BBMax.x - _BBMin.x;
		tmp.y = _BBMax.y;
		tmp.z = _BBMax.z - _BBMin.z;


		_children[0] = new QuadTreeNode; // bottomleft
		_children[0]->_BBMin = _BBMin;
		_children[0]->_BBMax.x = _BBMin.x + tmp.x / 2;
		_children[0]->_BBMax.y = _BBMax.y;
		_children[0]->_BBMax.z = _BBMin.z + tmp.z / 2;

		_children[1] = new QuadTreeNode; // upperLeft
		_children[1]->_BBMin = _BBMin;
		_children[1]->_BBMin.z = _BBMin.z + tmp.z / 2;
		_children[1]->_BBMax = _BBMax;
		_children[1]->_BBMax.x = _BBMin.x + tmp.x / 2;

		_children[2] = new QuadTreeNode; // upperRight
		_children[2]->_BBMin.x = _BBMin.x + tmp.x / 2;
		_children[2]->_BBMax.y = _BBMax.y;
		_children[2]->_BBMin.z = _BBMin.z + tmp.z / 2;
		_children[2]->_BBMax = _BBMax;

		_children[3] = new QuadTreeNode; // bottomRight
		_children[3]->_BBMin = _BBMin;
		_children[3]->_BBMin.x = _BBMin.x + tmp.x / 2;
		_children[3]->_BBMax = _BBMax;
		_children[3]->_BBMax.z = _BBMin.z + tmp.z / 2;


		Plane plane1;
		plane1.distance = -(_BBMin.x + tmp.x / 2);
		plane1.normal = glm::vec3(1.0f, 0.0f, 0.0f);
		PlaneResult boxStatus1;

		Plane plane2; // X-plane
		plane2.distance = -(_BBMin.z + tmp.z / 2);
		plane2.normal = glm::vec3(0.0f, 0.0f, 1.0f);
		PlaneResult boxStatus2;

		for (auto const& tmpData : data)
		{

			boxStatus1 = BBPlaneTest(tmpData.second->getModel().getBox(), plane1);

			boxStatus2 = BBPlaneTest(tmpData.second->getModel().getBox(), plane2);

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
			_children[i]->CreateNodes(childList[i], depth + 1, maxDepth);
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
	planes[4].normal.x = -(mat[0][3] - mat[0][2]);
	planes[4].normal.y = -(mat[1][3] - mat[1][2]);
	planes[4].normal.z = -(mat[2][3] - mat[2][2]);
	planes[4].distance = -(mat[3][3] - mat[3][2]);

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

void QuadTreeNode::TraverseTree(std::vector<int>& indices, Plane * planes, const int &nrOfPlanes)
{
	PlaneResult testResult;
	for (int i = 0; i < nrOfPlanes; i++)
	{
		testResult = BBPlaneTest(_BBMin, _BBMax, planes[i]);
		if (testResult != PlaneResult::Outside)
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
			TraverseTree(indices, planes, nrOfPlanes);
		}
	}
}

