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
#include "Game/Level/Path.h"

#define GUARDVIEWDISTANCE 9.f
#define GUARDFOV 75.0f * M_PIf / 180.f

class Guard : public GameObject
{
private:
	glm::vec3 _point1x; 
	glm::vec3 _point2x;
	glm::vec3 _point1z;
	glm::vec3 _point2z;
	glm::vec3 _guardsstartposition;
	EventManager*_eventManager;
	Character* _player;
	glm::vec3 _forward;
	Path *path;
	float _speed;
	float _distLength;
	int _aiChoice;
	int _height;
	int _width;
	Grid *_currentLevel;
	float _currentGridSpace;
	bool DetectedPlayer();
	float _currentRot;
public:
	virtual ~Guard();
	Guard();
	void print();
	void setLevel(Grid *level);
	void WalkingBetweenFourPoints(float dt);
	virtual	void update(float dt);
	Guard(Character* player, EventManager* event, Model & m, Grid * gridet);
	int randomgenerator(int randomNumber);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
};

