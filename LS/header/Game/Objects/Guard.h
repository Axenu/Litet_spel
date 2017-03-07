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
#include <Sound/Irrklang.h>

#define GUARDVIEWDISTANCE 20.f
#define GUARDHEARDISTANCE 20.f

#define LOOKNOISEINTRESTTIME 2.f
#define LOOKNOISELIMIT 0.4f

#define GUARDFOV 45.0f * M_PIf / 180.f

enum GuardState
{
	pathing = 0,
	looking
};



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
	float _noiseDetVal;
	float _finalDetVal;
	float _interestTime;
	glm::vec3 _pointOfInterest;
	GuardState _state;
	PointLightObject *_lantern;

	//Field of view in cosine
	float _detectFov;
	Level *_currentLevel;
	float _currentGridSpace;
	GuardState checkState(float dt);
	void setLookingState();
	void setPathingState();
	void noiseDetection(glm::vec3 pos, float dt, float noise, glm::vec4 noisePos);
	void visionDetection(glm::vec3 pos, float dt, float playerDist, glm::vec3 dirToPlayer);
	void finalDetection();
	float DetectedPlayer(float playerDist, glm::vec3 dirToPlayer);
	std::vector<glm::vec2> _walkingPoints;
public:
	virtual ~Guard();
	virtual	void update(float dt);
	glm::vec2 getNextPosition();
	PointLightObject *getLight();
	Guard(glm::vec3 position, Character* player, EventManager* event, Model& m, Level* Level, std::vector<glm::vec2>& walkingPoints);
	unsigned int _id;
};
