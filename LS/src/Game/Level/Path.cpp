#include "../../../header/Game/Level/Path.h"

int Path::randomgenerator(int randomNumber)
{
	return (int)rand() % randomNumber + 1;
}

bool Path::movePosition(glm::vec3 *position, glm::vec3 nextPoint, float speed, float dt)
{
	float distLength = glm::length(*position - nextPoint);
	float moveLen = speed * dt;
	bool reached = moveLen > distLength;
	moveLen = reached ? distLength : moveLen;
	*position += moveLen * glm::normalize(nextPoint - *position);
	return reached;
}

Path::Path(std::vector<glm::vec3> &path)
	: _index(path.size() - 1), _path(std::move(path))
{

}

Path::~Path()
{

}

bool Path::walkOnPath(glm::vec3 *position, float speed, float dt)
{
	if (_index >= 0)
	{
		if(movePosition(position, _path[_index], speed, dt)) //if moved to another square)
			_index--;
		return _index < 0;
	}
	return true;
}

glm::vec3 Path::movingTo() 
{
	return  _index < 0 ? glm::vec3() : _path[_index];
}

glm::vec3 Path::getDest()
{
	return _index < 0 ? glm::vec3() : _path[0];
}
