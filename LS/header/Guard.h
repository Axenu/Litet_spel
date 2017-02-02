#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"



class Guard : public GameObject
{
private:
	glm::vec3 point1x;
	glm::vec3 point2x;
	glm::vec3 point1z;
	glm::vec3 point2z;
	glm::vec3 guardsstartposition;
	int aiChoice;

public:
	virtual ~Guard();
	Guard();
	Guard(glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
	void onUpdate(float dt);
	Guard(Model & m, Grid * gridet);
	void setPositionfromMap(glm::vec3 Guarden);
};

