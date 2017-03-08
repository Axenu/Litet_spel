#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"

void Guard::update(float dt)
{

	glm::vec3 pos = _position;
	glm::vec3 dirToPlayer;
	float playerDist;

	dirToPlayer = _player->getWorldPos() - this->getWorldPos();
	playerDist = glm::length(dirToPlayer);
	dirToPlayer = glm::normalize(dirToPlayer);

	checkState(dt);

	switch (_state)
	{
	case GuardState::pathing:
		//True if no grid path exists/complete
		if (_path->walkOnPath(&pos, _speed, dt))
		{
			glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
			glm::ivec2 point;
			if (_walkPoints.hasPath())
				point = _walkPoints.getNextPosition();
			else if (_walkPoints.walkRandom())
				point = _currentLevel->getGrid().getRandomSquare();
			else if (_walkPoints.walkToEnd())
			{
				//Make the guard stop moving
				setStillState();
				break;
			}
			//Slightly average distance to search in grid
			int dist = std::max(std::abs(point.x - start.x), std::abs(point.y - start.y)) + 10;
			//Generate a new path
			_path = _currentLevel->getGrid().generatePath(start, point, dist);
		}
		setPosition(pos);
		face(_path->movingTo());
	
		sound.PlaySource3DSound(sound.GetSound("Resources/Sounds/GuardWalking.wav"), _player->getWorldPos(), this->getWorldPos(), _player->getForward(), _player->getUp(), _player->getVelocity());
		sound.Update();
		break;

	case GuardState::looking:
		face(_pointOfInterest);
		
		sound.Pause();
		break;
	}

	GameObject::update(dt); //Let object update the move vars before doing our detection logic

							//Get direction vector and distance to player
	
	visionDetection(pos, dt, playerDist, dirToPlayer);
	noiseDetection(pos, dt, _player->getNoise(), _player->getWorldPos());
	finalDetection();
}

Guard::Guard(glm::vec3 position, Character* player, EventManager* event, Model &m, Level *level, WalkPoints &walkingPoints) :
	GameObject(m), _player(player), _currentLevel(level), _walkPoints(std::move(walkingPoints))
{
	_eventManager = event;

	setPosition(position);
	_detectFov = std::cos(GUARDFOV);

	glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
	_path = _currentLevel->getGrid().generatePath(start, _currentLevel->getGrid().getRandomSquare(), 15);

	_speed = 0.4f;

	_detectionScore = 0.0f;

	_noiseDetVal = 0.0f;

	_finalDetVal = 0.0f;

	//setup lantern
	PointLightValue light(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f), 3.0f);
 	_lantern = new PointLightObject(light, this);
	_lantern->init();
	_lantern->setPosition(0.0f, 1.0f, 1.0f);
	addChild(_lantern);
}

Guard::~Guard()
{
}

void Guard::init()
{
	GameObject::init();
	if (_walkPoints.hasPath() || _walkPoints.walkRandom())
		setPathingState();
	else
		setStillState();
}

GuardState Guard::checkState(float dt)
{
	switch (_state)
	{
	case GuardState::pathing:
		if (_finalDetVal > LOOKNOISELIMIT)
		{
			setLookingState();
		}
		break;
	case GuardState::looking:
		_interestTime -= dt;
		if (_finalDetVal < LOOKNOISELIMIT)
		{
			if (_interestTime < 0.0f)
			{
				setPathingState();
			}

		}
		else
		{
			_interestTime = LOOKNOISEINTRESTTIME;
		}
		break;
	case GuardState::still:
		break;
	default:
		setStillState();
	}
	return _state;
}

void Guard::setStillState()
{
	_state = GuardState::still;
	face(_currentLevel->getGrid().getCenter(_walkPoints._faceDir));
	//Set properate animation
	_animatedSkel->setAnimPose("", 0.5f, 0.f);
}
void Guard::setLookingState()
{
	_state = GuardState::looking;
	//Set properate animation
	_animatedSkel->stopAnimation();
}

void Guard::setPathingState()
{
	_state = GuardState::pathing;
	//Set properate animation
	_animatedSkel->setAnim("", AnimatedSkeleton::Loop);
}

void Guard::noiseDetection(glm::vec3 pos, float dt, float noise, glm::vec4 noisePos)
{
	glm::vec3 dirToNoise = noisePos - this->getWorldPos();
	float noiseDist = glm::length(dirToNoise);
	//dirToNoise = glm::normalize(dirToNoise);
	float guardHearDist = noise * GUARDHEARDISTANCE;

	if (noiseDist < guardHearDist)
	{
		float noiseVal = (guardHearDist - noiseDist) / guardHearDist; // make between 0 - 1
		noiseVal = 2.0f * noiseVal - (noiseVal * noiseVal); // 2*x - x^2
		noiseVal *= dt * noise;
		_noiseDetVal += noiseVal;
		_noiseDetVal = std::fmin(_noiseDetVal, 1.0f); //clamp value to not be over 1.
	}
	else
	{
		if (_noiseDetVal > 0.0f)
		{
			_noiseDetVal -= dt * 0.1f;
		}

	}
}

void Guard::visionDetection(glm::vec3 pos, float dt, float playerDist, glm::vec3 dirToPlayer)
{
	//Get the distance that the guard can see the player based on light and obscuring objects
	float detectionDist = GUARDVIEWDISTANCE * DetectedPlayer(playerDist, dirToPlayer);

	//Activate the detection event based on distance between the player and guard
	if (playerDist <  detectionDist)
	{
		//Timer to determine the amount of time before the guard detects the player
		float detectionAmount = (playerDist / detectionDist);
		_detectionScore += dt * ((detectionAmount > 0.2f) ? detectionAmount : 0.2f);
	}
	else
	{
		if (_detectionScore > 0.0f)
		{
			_detectionScore -= dt*0.1f;
		}
	}
}

void Guard::finalDetection()
{
	_finalDetVal = _noiseDetVal + _detectionScore;
	if (_finalDetVal > 1.0f)
	{
		GameOverEvent event(false);
		_eventManager->execute(event);
		sound.PlaySource2DSound(sound.GetSound("Resources/Sounds/Gameover.wav"), false);
		sound.SetVolume(0.5f);
	}
	else
	{
		GuardAlertEvent event(this->getWorldPos(), _finalDetVal);
		event._id = _id;
		_eventManager->execute(event);
	}
	if (_finalDetVal < LOOKNOISELIMIT)
	{
		_pointOfInterest = _player->getWorldPos();
	}
}

float Guard::DetectedPlayer(float playerDist, glm::vec3 dirToPlayer)
{
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 pos = this->getWorldPos();

	pos.y = 1.3f;

	//Player to close detect!
	if (playerDist < 1.2f)
		return 1.0f;

	if (glm::dot(dirToPlayer, getForward()) > _detectFov)
	{
		//Get light at position
		float playerLight = _player->getLightAtPosition();

		//If player behind wall or obscuring object, not detected
		if (_currentLevel->getGrid().getDist(pos, dirToPlayer, playerDist) || _currentLevel->getGrid().getDist(pos, dirToPlayer, playerDist, _player->getEyePos(), object))
		{
			return 0.0f;
		}
		else
		{
			return playerLight;
		}
	}
	return 0.0f;
}

PointLightObject *Guard::getLight()
{
	return _lantern;
}
