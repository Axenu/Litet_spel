#pragma once
#define GLM_FORCE_RADIANS
#include<glm/vec2.hpp>

enum gridType
{
	// color values (0, 0, 0) = nothing, (255, 255, 255) = wall, (0, 255, 0) = loot, (0, 0, 255) = door (255, 0,0) = exiting
	nothing = 0, 
	wall = 1, 
	object = 2,
	tresure = 3, 
	loot = 4, 
	door = 5, 
	exiting = 6, 
};

/* Simple struct holding information of a grid square
*/
struct GridSquare {
	glm::ivec2 _square;
	gridType _grid;

	GridSquare()
		: _square(), _grid(gridType::wall) {}
	GridSquare(glm::ivec2 square, gridType grid)
		: _square(square), _grid(grid) {}
};
