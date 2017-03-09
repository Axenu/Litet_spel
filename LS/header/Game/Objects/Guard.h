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
#include "Sound/SoundManager.h"
#include "WalkPoints.h"

#define GUARDVIEWDISTANCE 20.f
#define GUARDHEARDISTANCE 20.f

#define LOOKNOISEINTRESTTIME 4.0f
#define SEARCHINTERESTTIME 1.0f
#define LOOKNOISELIMIT 0.4f
#define SEARCHNOISELIMIT 0.6f

#define GUARDFOV 45.0f * M_PIf / 180.f

enum GuardState
{
	pathing = 0,
	still = 1,
	looking = 2,
	searching = 3,
	returning = 4
};



class Guard : public GameObject
{
private:
	EventManager*_eventManager;
	Character* _player;
	std::shared_ptr<Path> _path;
	WalkPoints _walkPoints;
	float _speed;
	float _visionDetScore;
	float _lastVisionVal;
	float _noiseDetVal;
	float _finalDetVal;
	float _interestTime;
	float _lastNoiseVal;
	glm::vec3 _pointOfInterest;
	glm::vec3 _returnPoint;
	GuardState _state;

	irrklang::ISound *_walkingSound;

	//Field of view in cosine
	float _detectFov;
	Level *_currentLevel;
	GuardState checkState(float dt);
	void setStillState();
	void setLookingState();
	void setPathingState();
	void setSearchingState();
	void setReturningState();
	void setPath(glm::vec3 start, glm::vec3 point);
	void noiseDetection(glm::vec3 pos, float dt, float noise, glm::vec4 noisePos);
	void visionDetection(glm::vec3 pos, float dt, float playerDist, glm::vec3 dirToPlayer);
	void finalDetection();
	float DetectedPlayer(float playerDist, glm::vec3 dirToPlayer);
public:
	Guard(glm::vec3 position, Character* player, EventManager* event, Model & m, Level* Level, WalkPoints& walkingPoints);
	virtual ~Guard();
	virtual void init();
	virtual	void update(float dt);
	unsigned int _id;
};
