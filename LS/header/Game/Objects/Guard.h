#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"
#include <stdlib.h>
#include <time.h>


class Guard : public GameObject
{
private:
	glm::vec3 _point1x; //höjden först sen breden. 
	glm::vec3 _point2x; //höjden först sen breden. 
	glm::vec3 _point1z; //höjden först sen breden. 
	glm::vec3 _point2z; //höjden först sen breden. 
	glm::vec3 _guardsstartposition; //höjden först sen breden. 
	gridValues** _levalues;
	int _aiChoice;
	int _heightLength;
	int _widthLength;
	int _height;
	int _width;
	float _currentGridSpace;

public:
	virtual ~Guard();
	Guard();
	void print();
	std::vector<glm::ivec2> gridWalkingBetweenTwoPoints(glm::ivec2 GoalPosition);
	glm::vec3 roundTheValuefrom0Comma01(glm::vec3);
	Guard(glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
	virtual	void update(float dt);
	Guard(Model & m, Grid * gridet);
	int randomgenerator(int randomNumber);
	void buildgridarray(Grid * gridet, unsigned int sizeX, unsigned int sizeY);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
};

