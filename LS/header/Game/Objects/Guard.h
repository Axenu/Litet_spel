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

#define GUARDVIEWDISTANCE 6.f
#define GUARDFOV 45.0f * M_PIf / 180.f
#define ALMOSTDETECTFOV 75.0f * M_PIf / 180.0f

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
	float _almostDetectFov;
	Level *_currentLevel;
	unsigned int sizeOfVector;
	float _currentGridSpace;
	bool DetectedPlayer();
	bool AlmostDetected(glm::vec3(dirToPlayer), float viewDist);
	std::vector<glm::vec2>* walkingPoints;
	std::vector<glm::vec2> walkingPointsen;
public:
	virtual ~Guard();
	virtual	void update(float dt);
	glm::vec2 getNextPosition();
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Level *level, std::vector<glm::vec2>* walkingPoints);
};

