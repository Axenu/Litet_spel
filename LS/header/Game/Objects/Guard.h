#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "Game/Level/Level.h"
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
	EventManager*_eventManager;
	Character* _player;
	std::shared_ptr<Path> _path;
	float _speed;
	float _distLength;
	/* Field of view in cosine
	*/
	unsigned int _whatPathToLoad;
	float _detectFov;
	Level *_currentLevel;
	unsigned int sizeOfVector;
	float _currentGridSpace;
	bool DetectedPlayer();
	std::vector<glm::vec2>* walkingPoints;
	std::vector<glm::vec2> walkingPointsen;
public:
	virtual ~Guard();
	void setLevel(Level *level);
	virtual	void update(float dt);
	glm::vec2 getNextPosition();
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Level* Level, std::vector<glm::vec2>* walkingPoints);
};

