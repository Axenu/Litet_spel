#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"

void Guard::update(float dt)
{

	glm::vec3 pos = _position;
	glm::vec3 dirToPlayer;
	float playerDist;

	checkState(dt);

	switch (_state)
	{
	case GuardState::pathing:
		if (_path->walkOnPath(&pos, _speed, dt))
		{
			glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
			if (_walkingPoints.size() == 0)
				_path = _currentLevel->getGrid().generatePath(start, _currentLevel->getGrid().getSquare(getWorldPos()), 15);//		_path = _currentLevel->generatePath(start, _currentLevel->getRandomSquare());
			else
				_path = _currentLevel->getGrid().generatePath(start, getNextPosition(), 15);
		}
		if (pos.x < 0 || pos.z < 0)
			int a = 0;
		setPosition(pos);
		face(_path->movingTo());
		break;
	case GuardState::looking:
		face(_pointOfInterest);
		break;
	}

	GameObject::update(dt); //Let object update the move vars before doing our detection logic

							//Get direction vector and distance to player
	dirToPlayer = _player->getWorldPos() - this->getWorldPos();
	playerDist = glm::length(dirToPlayer);
	dirToPlayer = glm::normalize(dirToPlayer);
	visionDetection(pos, dt, playerDist, dirToPlayer);
	noiseDetection(pos, dt, _player->getNoise(), _player->getWorldPos());
}

glm::vec2 Guard::getNextPosition()
{
	_whatPathToLoad += 1;
	if (_whatPathToLoad >= _walkingPoints.size())
		_whatPathToLoad = 0;
	return(_walkingPoints[_whatPathToLoad]);
}

Guard::Guard(glm::vec3 position, Character* player, EventManager* event, Model &m, Level *level, std::vector<glm::vec2>& walkingPoints) :
	GameObject(m), _player(player), _currentLevel(level), _walkingPoints(std::move(walkingPoints))
{
	_eventManager = event;
	_whatPathToLoad = 0;

	setPosition(position);
	_detectFov = std::cos(GUARDFOV);

	glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
	_path = _currentLevel->getGrid().generatePath(start, _currentLevel->getGrid().getRandomSquare(), 15);

	_speed = 0.4f;

	_detectionScore = 0.3f;

	_noiseDetVal = 0.0f;

	_state = GuardState::pathing;
}

Guard::~Guard()
{
}

GuardState Guard::checkState(float dt)
{
	switch (_state)
	{
	case GuardState::pathing:
		if (_noiseDetVal > LOOKNOISELIMIT)
		{
			setLookingState();
		}
		break;
	case GuardState::looking:
		_interestTime -= dt;
		if (_noiseDetVal < LOOKNOISELIMIT)
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
	default:
		setLookingState();
	}
	return _state;
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
		_pointOfInterest = noisePos;
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
		float detectionAmount = (1.0f - (playerDist / detectionDist));
		_detectionScore -= dt * ((detectionAmount > 0.2f) ? detectionAmount : 0.2f);

		if (_detectionScore < 0.0f)
		{
			//Game over
			GameOverEvent event(false);
			_eventManager->execute(event);
		}
		else
		{
			// std::cout << "detection" << std::endl;
			GuardAlertEvent event(pos, 1.0f - _detectionScore * 3.3333333333f);
			event._id = _id;
			_eventManager->execute(event);
			// ALmost detected
			// call event
			// use detection value
			// use pos
			// use VP-Matrix?
		}
	}
	else
	{
		if (_detectionScore <= 0.3f)
		{
			// std::cout << _detectionScore << std::endl;
			_detectionScore += dt*0.1f;
			GuardAlertEvent event(pos, 1.0f - _detectionScore * 3.3333333333f);
			event._id = _id;
			_eventManager->execute(event);
		}
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
