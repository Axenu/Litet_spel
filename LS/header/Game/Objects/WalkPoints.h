#pragma once
#include <glm/vec2.hpp>
#include <vector>

class WalkPoints
{
	std::vector<glm::ivec2> _points;
	/* Type of walk:
	0: Stand still/Stop at end
	1: Start random walk at end
	2: Loop path
	*/
	unsigned int _walkType;
	unsigned int _current;
public:

	/* When standing still this is the point the guard will face at end */
	glm::ivec2 _faceDir;

	WalkPoints()
		: _points(), _walkType(0), _current(0)
	{
	}
	WalkPoints(std::vector<glm::ivec2> &points, unsigned int walkType, glm::ivec2 endFace)
		: _points(std::move(points)), _walkType(walkType), _current(0), _faceDir(endFace)
	{
		//Verify walk type is possible
		if (_points.size() < 2 && _walkType > 1)
			_walkType = 0;
	}
	/* Returns if guard should randomize path when path complete */
	bool walkRandom()
	{
		return _walkType == 1;
	}
	/* Returns true if guard should stop moving at end of path */
	bool walkToEnd()
	{
		return _walkType == 0;
	}
	bool loopPath()
	{
		return _walkType == 2;
	}

	glm::ivec2 getNextPosition()
	{
		//Normal 'go to next square'
		if (_current < _points.size())
			return _points[_current++];
		//Loop path
		else if (_walkType == 2)
		{
			_current = 0;
			return _points[0];
		}
		//Error
		return glm::ivec2(0);
	}
	bool hasPath()
	{
		return _current < _points.size() || loopPath();
	}
};