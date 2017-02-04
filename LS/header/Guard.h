#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"
#include <stdlib.h>
#include <time.h>


class Guard : public GameObject
{
private:
	glm::vec3 point1x; //höjden först sen breden. 
	glm::vec3 point2x; //höjden först sen breden. 
	glm::vec3 point1z; //höjden först sen breden. 
	glm::vec3 point2z; //höjden först sen breden. 
	glm::vec3 guardsstartposition; //höjden först sen breden. 
	gridValues** levalues;
	int aiChoice;
	int _heightLength;
	int _widthLength;
	int height;
	int width;

public:
	virtual ~Guard();
	Guard();
	glm::vec3 roundTheValuefrom0Comma01(glm::vec3);
	Guard(glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
virtual	void update(float dt);
	Guard(Model & m, Grid * gridet);
	int randomgenerator();
	void buildgridarray();
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
};

