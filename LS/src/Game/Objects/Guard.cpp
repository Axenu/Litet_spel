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
		_walkingSound->play();
		break;

	case GuardState::looking:
		face(_pointOfInterest);
		_walkingSound->pause();
		break;
	case GuardState::searching:
		if (_path->walkOnPath(&pos, _speed, dt))
		{
			setLookingState();
		}
		setPosition(pos);
		face(_pointOfInterest);
		_walkingSound->play();
		break;
	case GuardState::returning:
		if (_path->walkOnPath(&pos, _speed, dt))
		{
			setPathingState();
		}
		setPosition(pos);
		face(_path->movingTo());
		_walkingSound->play();
		break;
	default:
		break;
	}
	_walkingSound->setPosition(this->getWorldPos());

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

	_speed = 0.6f;

	_visionDetScore = 0.0f;

	_noiseDetVal = 0.0f;

	_finalDetVal = 0.0f;

	_lastNoiseVal = 0.0f;

	_lastVisionVal = 0.0f;

	_walkingSound = SoundManager::getInstance().play3DSound(GUARD_WALK, this->getWorldPos(), true, false);
}

Guard::~Guard()
{
	if (_walkingSound)
		_walkingSound->drop();
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
				if (_finalDetVal > LOOKNOISELIMIT)
				{
					setSearchingState();
				}
				else
				{
					setReturningState();
				}
			}

		}
		else if (_finalDetVal < SEARCHNOISELIMIT)
		{
			if (_interestTime < 0.0f)
			{
				setSearchingState();
			}

		}
		else
		{
			setSearchingState();
		}
		break;
	case GuardState::still:
		if (_finalDetVal > LOOKNOISELIMIT)
		{
			setLookingState();
		}
		break;
	case GuardState::searching:
		_interestTime -= dt;
		if (_interestTime < 0.0f)
		{
			glm::ivec2 dest = _currentLevel->getGrid().getSquare(_path->getDest());
			glm::ivec2 POISquare = _currentLevel->getGrid().getSquare(_pointOfInterest);
			if (dest == POISquare)
			{
				setSearchingState();
			}
		}
		break;
	case GuardState::returning:
		if (_finalDetVal > LOOKNOISELIMIT)
		{
			setLookingState();
		}
		break;
	default:
		setStillState();
	}
	return _state;
}

void Guard::setStillState()
{

	face(_currentLevel->getGrid().getCenter(_walkPoints._faceDir));
	//Set properate animation
	if (hasSkeleton())
		_animatedSkel->setAnimPose("", 0.5f, 0.f);

	_state = GuardState::still;
}
void Guard::setLookingState()
{
	//Set properate animation
	if (hasSkeleton())
		_animatedSkel->stopAnimation();
	_interestTime = LOOKNOISEINTRESTTIME;
	switch (_state)
	{
	case GuardState::pathing:
		_returnPoint = getWorldPos();
		break;
	case GuardState::still:
		_returnPoint = getWorldPos();
		break;
	default:
		break;
	}
	_state = GuardState::looking;
}

void Guard::setPathingState()
{

	//Set properate animation
	if (hasSkeleton())
		_animatedSkel->setAnim("", AnimatedSkeleton::Loop);

	_state = GuardState::pathing;
}

void Guard::setSearchingState()
{
	//Set properate animation

	switch (_state)
	{
	case GuardState::searching:
		break;
	default:
		if (hasSkeleton())
			_animatedSkel->setAnim("", AnimatedSkeleton::Loop);
	}
	_interestTime = SEARCHINTERESTTIME;

	setPath(this->getWorldPos(), _pointOfInterest);
	_state = GuardState::searching;
}

void Guard::setReturningState()
{
	if (hasSkeleton())
		_animatedSkel->setAnim("", AnimatedSkeleton::Loop);

	setPath(this->getWorldPos(), _returnPoint);

	_state = GuardState::returning;
}

void Guard::setPath(glm::vec3 start, glm::vec3 point)
{
	glm::ivec2 iStart = _currentLevel->getGrid().getSquare(start);
	glm::ivec2 iPoint = _currentLevel->getGrid().getSquare(point);

	int dist = std::max(std::abs(point.x - start.x), std::abs(point.y - start.y)) + 10;

	_path = _currentLevel->getGrid().generatePath(iStart, iPoint, dist);
}

void Guard::noiseDetection(glm::vec3 pos, float dt, float noise, glm::vec4 noisePos)
{
	glm::vec3 dirToNoise = noisePos - this->getWorldPos();
	float noiseDist = glm::length(dirToNoise);
	//dirToNoise = glm::normalize(dirToNoise);
	float guardHearDist = noise * GUARDHEARDISTANCE;

	if (noiseDist < guardHearDist)
	{
		_lastNoiseVal = (guardHearDist - noiseDist) / guardHearDist; // make between 0 - 1
		_lastNoiseVal = 2.0f * _lastNoiseVal - (_lastNoiseVal * _lastNoiseVal); // 2*x - x^2
		_lastNoiseVal *= dt * noise;
		_noiseDetVal += _lastNoiseVal;
		_noiseDetVal = std::fmin(_noiseDetVal, 1.0f); //clamp value to not be over 1.
	}
	else
	{
		if (_noiseDetVal > 0.0f)
		{
			_noiseDetVal -= dt * 0.1f;
		}
		_lastNoiseVal = 0.0f;
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
		_visionDetScore += dt * ((detectionAmount > 0.2f) ? detectionAmount : 0.2f);
		_lastVisionVal = detectionAmount;
	}
	else
	{
		if (_visionDetScore > 0.0f)
		{
			_visionDetScore -= dt*0.1f;
		}
		_lastVisionVal = 0.0f;
	}
}

void Guard::finalDetection()
{
	_finalDetVal = _noiseDetVal + _visionDetScore;

	if (_finalDetVal > 1.0f || distanceDetection())
	{
		GameOverEvent event(false);
		_eventManager->execute(event);
		Sound *tempSound = SoundManager::getInstance().play2DSound(LOSE_SOUND, false, false);
		tempSound->setVolume(0.5f);
		tempSound->play();
	}
	else
	{
		GuardAlertEvent event(this->getWorldPos(), _finalDetVal);
		event._id = _id;
		_eventManager->execute(event);
	}
	if (_lastNoiseVal > 0.0f || _lastVisionVal > 0.0f)
	{
		_pointOfInterest = _player->getWorldPos();
	}
}

float Guard::DetectedPlayer(float playerDist, glm::vec3 dirToPlayer)
{
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 pos = this->getWorldPos();

	pos.y = 1.3f;

	if (glm::dot(dirToPlayer, getForward()) > _detectFov)
	{
		//Get light at position
		float playerLight = _player->getLightAtPosition();

		//If player behind wall or obscuring object, not detected
		if (_currentLevel->getGrid().getDist(pos, dirToPlayer, playerDist, wall) || _currentLevel->getGrid().getDist(pos, dirToPlayer, playerDist, glm::vec3(_player->getEyePos().x, _player->getHeight(), _player->getEyePos().z), object))
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

bool Guard::distanceDetection()
{
	float playerDist = glm::length(this->getWorldPos() - _player->getWorldPos());

	//Player to close detect!
	if (playerDist < 0.8f)
		return true;

	return false;
}
