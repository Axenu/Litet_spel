#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "GridDataStructure.h"

class Path
{
private:
	std::vector<glm::ivec2> _path;
	Grid * _level;
	int randomgenerator(int randomNumber);
	bool movePosition(glm::vec3 *position, glm::ivec2 nextSquare, float speed, float dt);
public:
	Path();
	~Path();
	void createPath(glm::ivec2 startPosition, glm::ivec2 goalPosition);
	void setLevel(Grid * level);
	bool walkOnPath(glm::vec3 *position, glm::vec3 &movingTo, float speed, float dt);
};