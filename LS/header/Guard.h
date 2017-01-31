#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"

class Guard : public GameObject
{
private:
	glm::vec3 Guardsposition;
	glm::vec3 point1;
	glm::vec3 point2;
public:
	~Guard();
	Guard();
	Guard(glm::vec3 Guarden);
	void WalkingBetweenTwoPoints();
	void Update(int dt);
};

