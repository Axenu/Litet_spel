#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/* Simple class for traveling on a grid
*/
class GridTraveler
{
private:
	int _signX, _signY;
	float _GRIDSPACING;
	glm::ivec2 _gridP;
	glm::vec2 _dir, _invDir;
	//Distance to the edge of next grid square in x,y (2D)
	glm::vec2 _nEdge;
public:
	GridTraveler(float GRIDSPACING, glm::ivec2 square, glm::vec3 pos, glm::vec3 dir);

	float goNext();

	glm::ivec2 getSquare();
	~GridTraveler();
};

