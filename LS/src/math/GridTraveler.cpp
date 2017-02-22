#include "math/GridTraveler.h"
#include <glm/geometric.hpp>



GridTraveler::GridTraveler(float GRIDSPACING, glm::ivec2 square, glm::vec3 pos, glm::vec3 dir)
	: _GRIDSPACING(GRIDSPACING), _gridP(square)
{
	_dir = glm::normalize(glm::vec2(dir.x, dir.z));
	_signX = (_dir.x > 0) - (_dir.x < 0);
	_signY = (_dir.y > 0) - (_dir.y < 0);
	_invDir = glm::vec2(1 / _dir.x, 1 / _dir.y);

	//Calc initial distance to edges
	_nEdge.x = (_gridP.x + (_signX > 0)) * GRIDSPACING - pos.x;
	_nEdge.y = (_gridP.y + (_signY > 0)) * GRIDSPACING - pos.z;
}


GridTraveler::~GridTraveler()
{
}

float GridTraveler::goNext()
{
	//Calc dist to next edge
	glm::vec2 nDist = _invDir * _nEdge;
	if (nDist.x < nDist.y) //True: X edge is closest
	{
		//Pass over to next x square:
		_nEdge.y += _signX * _GRIDSPACING;
		_gridP.y += _signX;
		//Adjust dist to next edge x,y
		_nEdge -= nDist.x * _dir;
		return nDist.x;
	}
	else
	{
		//Pass over to next y square:
		_nEdge.y += _signY * _GRIDSPACING;
		_gridP.y += _signY;
		//Adjust dist to next edge x,y
		_nEdge -= nDist.y * _dir;
		return nDist.y;
	}
}

glm::ivec2 GridTraveler::getSquare()
{
	return _gridP;
}