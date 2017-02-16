#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "Character.h"
#include <stdlib.h>
#include <time.h>
#include "intersectionFunctions.h"

#define GUARDVIEWDISTANCE 3.f

class Guard : public GameObject
{
private:
	glm::vec3 _point1x; //h�jden f�rst sen breden. 
	glm::vec3 _point2x; //h�jden f�rst sen breden. 
	glm::vec3 _point1z; //h�jden f�rst sen breden. 
	glm::vec3 _point2z; //h�jden f�rst sen breden. 
	glm::vec3 _guardsstartposition; //h�jden f�rst sen breden. 
	EventManager*_eventManager;
	Character* _player;
	glm::vec3 _forward;
	float _speed;
	float _distLength;
	int _aiChoice;
	int _heightLength;
	int _widthLength;
	int _height;
	Grid *_currentLevel;
	int _width;
	glm::vec3 _displacement;
	float _currentGridSpace;
	std::vector<glm::vec2> _currentPath;
	bool DetectedPlayer();
	float _currentRot = 0.f; //deans kod

public:
	virtual ~Guard();
	Guard();
	void print();
	void StartGridBuild();
	void setLevel(Grid *level);
	std::vector<glm::vec2> generatingPath(glm::ivec2 GoalPosition);
	bool walkingInThePaths(float dt);
	glm::vec2 roundTheValuefrom0Comma01(glm::vec3);
	Guard(Character* player, EventManager* event, glm::vec3 Guarden,glm::vec3 Positonxy[4]);
	void WalkingBetweenFourPoints(float dt);
	virtual	void update(float dt);
	Guard(Character* player, EventManager* event, Model & m, Grid * gridet);
	int randomgenerator(int randomNumber);
//	void buildgridarray(Grid * gridet, unsigned int sizeX, unsigned int sizeY);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
	void goToSquare(float dt, glm::vec2 walkToSquare);
};

