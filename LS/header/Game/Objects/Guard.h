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
#include "Sound/Irrklang.h"

#define GUARDVIEWDISTANCE 10.f
#define GUARDFOV 45.0f * M_PIf / 180.f

class Guard : public GameObject
{
private:
	EventManager*_eventManager;
	Character* _player;
	std::shared_ptr<Path> _path;
	float _speed;
	float _distLength;
	unsigned int _whatPathToLoad;
	float _detectionScore;
	float _guardtimer;
	IrrKlang _guardSound;
	//Field of view in cosine
	float _detectFov;
	Level *_currentLevel;
	float _currentGridSpace;
	float DetectedPlayer(float playerDist, glm::vec3 dirToPlayer);
	std::vector<glm::vec2> _walkingPoints;
public:
	virtual ~Guard();
	virtual	void update(float dt);
	glm::vec2 getNextPosition();
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Level* Level, std::vector<glm::vec2>& walkingPoints);
};

