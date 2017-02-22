#include "math/GridTraveler.h"
#include <glm/geometric.hpp>
#include "math/MathFunctions.h"

const float LargeF = 10000000.f;

GridTraveler::GridTraveler(float GRIDSPACING, glm::ivec2 square, glm::vec3 pos, glm::vec3 dir)
	: _GRIDSPACING(GRIDSPACING), _gridP(square)
{
	_dir = glm::normalize(glm::vec2(dir.x, dir.z));
	_signX = sign(_dir.x);
	_signY = sign(_dir.y);

	//Calc initial distance to edges, signs between nEdge distance and dir is matched so: dir * nEdge -> +
	_nEdge.x = (_gridP.x + (_signX > 0)) * GRIDSPACING - pos.x;
	_nEdge.y = (_gridP.y + (_signY > 0)) * GRIDSPACING - pos.z;
	//Calc inv dir, relates to div by zer0 and the matches sign to the nEdge sign.
	_invDir = glm::vec2((_dir.x != 0 ? 1 / _dir.x : LargeF * sign(_nEdge.x)), (_dir.y != 0 ? 1 / _dir.y : LargeF  * sign(_nEdge.y)));
}


GridTraveler::~GridTraveler()
{
}

float GridTraveler::goNext()
{
	//Calc dist to next edges
	glm::vec2 nDist = _invDir * _nEdge;
	if (nDist.x < nDist.y) //Go to closest
	{
		//Pass over to next x square:
		_nEdge.x += _signX * _GRIDSPACING;
		_gridP.x += _signX;
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