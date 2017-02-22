#pragma once
#include "gl/glInclude.h"
#include "GameObject.h"
#include "Game/Level/Grid.h"
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
	float _detectFov;
	Grid *_currentLevel;
	float _currentGridSpace;
	bool DetectedPlayer();
public:
	virtual ~Guard();
	void setLevel(Grid *level);
	virtual	void update(float dt);
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Grid * gridet);
};

