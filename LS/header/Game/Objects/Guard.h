#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "GridDataStructure.h"
#include "Event/Input/InputManager.h"
#include "Event/EventManager.h"
#include "Character.h"
#include <stdlib.h>
#include <time.h>
#include "Game/Level/Path.h"
#include <memory>

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
	std::shared_ptr<Path> _path;
	float _speed;
	float _distLength;
	int _aiChoice;
	Grid *_currentLevel;
	float _currentGridSpace;
	bool DetectedPlayer();
public:
	virtual ~Guard();
	void print();
	void setLevel(Grid *level);
	void WalkingBetweenFourPoints(float dt);
	virtual	void update(float dt);
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Grid * gridet);
	void setPositionfromMap(glm::vec3 Guarden);
	void goToSquare(float dt, glm::vec3 walkTo); 
};

