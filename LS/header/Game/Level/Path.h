#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Path
{
private:
	int _index;
	std::vector<glm::vec3> _path;
	int randomgenerator(int randomNumber);
	bool movePosition(glm::vec3 *position, glm::vec3 nextSquare, float speed, float dt);
public:
	Path(std::vector<glm::vec3> &path);
	~Path();
	bool walkOnPath(glm::vec3 *position, float speed, float dt);
	/* Get the position next on the path */
	glm::vec3 movingTo();
	glm::vec3 getDest();
};