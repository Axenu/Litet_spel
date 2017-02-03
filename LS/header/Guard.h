#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"



class Guard : public GameObject
{
private:
	glm::vec3 point1x; //h�jden f�rst sen breden. 
	glm::vec3 point2x; //h�jden f�rst sen breden. 
	glm::vec3 point1z; //h�jden f�rst sen breden. 
	glm::vec3 point2z; //h�jden f�rst sen breden. 
	glm::vec3 guardsstartposition; //h�jden f�rst sen breden. 
	int aiChoice;

public:
	virtual ~Guard();
	Guard();
	Guard(glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
virtual	void update(float dt);
	Guard(Model & m, Grid * gridet);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo);
};

