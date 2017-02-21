#include "../../../header/Game/Level/Path.h"

int Path::randomgenerator(int randomNumber)
{
	return (int)rand() % randomNumber + 1;
}

bool Path::movePosition(glm::vec3 *position, glm::ivec2 nextSquare, float speed, float dt)
{
	glm::vec2 distance = glm::vec2(nextSquare.x + GRIDSPACE / 2.f, nextSquare.y + GRIDSPACE / 2.f) - glm::vec2(position->x, position->z);
	float distLength = glm::length(distance);
	if (distance.y > 0.f)
	{
		if (distLength < speed * dt)
		{
			position->z = (float)nextSquare.y + GRIDSPACE / 2.f;
			return true;
		}
		else
		{
			*position += glm::vec3(0.f, 0.f, speed * dt);
			return false;
		}
	}

	if (distance.y < 0.f)
	{
		if (distLength < speed * dt)
		{
			position->z = (float)nextSquare.y + GRIDSPACE / 2.f;
			return true;
		}
		else
		{
			*position -= glm::vec3(0.f, 0.f, speed * dt);
			return false;
		}
	}

	if (distance.x > 0.f)
	{
		if (distLength < speed * dt)
		{
			position->x = (float)nextSquare.x + GRIDSPACE / 2.f;
			return true;
		}
		else
		{
			*position += glm::vec3(speed * dt, 0.f, 0.f);
			return false;
		}
	}

	if (distance.x < 0.f)
	{
		if (distLength < speed * dt)
		{
			position->x = (float)nextSquare.x + GRIDSPACE / 2.f;
			return true;
		}
		else
		{
			*position -= glm::vec3(speed * dt, 0.f, 0.f);
			return false;
		}
	}
	return true;
}

Path::Path()
{

}

Path::~Path()
{

}

void Path::createPath(glm::ivec2 startPosition, glm::ivec2 goalPosition)
{
	_path = _level->generatePath(startPosition, goalPosition);
}

void Path::setLevel(Grid * level)
{
	this->_level = level;
}

bool Path::walkOnPath(glm::vec3 *position, glm::vec3 *forward, float *rotation, float speed, float dt)
{
	if (_path.size() == 0)
	{
		return true;
	}
	if (movePosition(position, _path[_path.size() - 1], speed, dt)) //if moved to another square
	{
		if (_path.size() > 1)
		{
			glm::vec2 currentSquare = _path[_path.size() - 1];
			glm::vec2 nextSquare = _path[_path.size() - 2];

			glm::vec2 difference = (currentSquare - nextSquare) * -1.f;
			difference = glm::normalize(difference);

			*forward = glm::vec3(difference.x, 0.0f, difference.y);

			float angle = acos(forward->z);

			if (forward->x < 0.f)
			{
				angle = M_2PIf - angle;
			}

			*rotation = angle;
		}
		_path.erase(_path.begin() + _path.size() - 1);
	}
	return false;
}
