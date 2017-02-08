#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "Character.h"
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
	EventManager*_eventManager;
	Character* _player;
	int _aiChoice;
	int _heightLength;
	int _widthLength;
	int _height;
	int _width;

	bool DetectedPlayer();

public:
	virtual ~Guard();
	Guard();
	void print();
	void gridWalkingBetweenTwoPoints(glm::vec3 GoalPosition);
	glm::vec3 roundTheValuefrom0Comma01(glm::vec3);
	Guard(Character* player, EventManager* event, glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
	virtual	void update(float dt);
	Guard(Character* player, EventManager* event, Model & m, Grid * gridet);
	int randomgenerator();
	void buildgridarray(Grid * gridet);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
};

