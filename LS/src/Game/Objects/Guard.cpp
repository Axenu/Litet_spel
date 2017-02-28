#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"

void Guard::update(float dt)
{
	
	glm::vec3 pos = _position;
	
	if (_path->walkOnPath(&pos, _speed, dt))
	{
		glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
		if (sizeOfVector < 1)
			_path = _currentLevel->getGrid().generatePath(start, _currentLevel->getGrid().getSquare(getWorldPos()));//		_path = _currentLevel->generatePath(start, _currentLevel->getRandomSquare());
		else
			_path = _currentLevel->getGrid().generatePath(start, getNextPosition());
	}
	if (pos.x < 0 || pos.z < 0)
		int a = 0;
	setPosition(pos);
	face(_path->movingTo());
	
	face(_player->getWorldPos());
	GameObject::update(dt); //Let object update the move vars before doing our update logic

	//Get direction vector and distance to player 
	glm::vec3 dirToPlayer = _player->getWorldPos() - this->getWorldPos();
	float playerDist = glm::length(dirToPlayer);
	dirToPlayer = glm::normalize(dirToPlayer);

	//Get the distance that the guard can see the player based on light and obscuring objects
	float detectionDist = GUARDVIEWDISTANCE * DetectedPlayer(playerDist, dirToPlayer);

	//Activate the detection event based on distance between the player and guard
	if (playerDist <  detectionDist)
	{
		std::cout << "Almost detected" << std::endl;
		
		//Timer to determine the amount of time before the guard detects the player
		float detectionAmount = (1.0f - (playerDist / detectionDist));
		_detectionScore -= dt * ((detectionAmount > 0.2f) ? detectionAmount : 0.2f);

		if (_detectionScore < 0.0f)
		{
			std::cout << "Detected" << std::endl;

			_detectionScore = 0.3f;
		}
	}

}

glm::vec2 Guard::getNextPosition()
{
	_whatPathToLoad += 1;
	if (_whatPathToLoad > sizeOfVector - 1)
		_whatPathToLoad = 0;
	return(walkingPointsen[_whatPathToLoad]);

	glm::vec2 test = (*walkingPoints)[_whatPathToLoad];
	return (*walkingPoints)[_whatPathToLoad];
}

Guard::Guard(glm::vec3 position, Character* player, EventManager* event, Model &m, Level *level, std::vector<glm::vec2>* walkingPoints) :
	GameObject(m), _player(player), _eventManager(event), _currentLevel(level)
{
	this->walkingPoints = walkingPoints;
	sizeOfVector = walkingPoints->size();
	_whatPathToLoad = 0;
	
	for (unsigned int i = 0; i < sizeOfVector; i++)
	{
		walkingPointsen.push_back((*walkingPoints)[i]);
	}

	setPosition(position);
	_detectFov = std::cos(GUARDFOV);

	glm::ivec2 start = _currentLevel->getGrid().getSquare(this->getWorldPos());
	_path = _currentLevel->getGrid().generatePath(start, _currentLevel->getGrid().getRandomSquare());

	_speed = 0.4f;

	_detectionScore = 0.3f;
}

Guard::~Guard()
{
}

float Guard::DetectedPlayer(float playerDist, glm::vec3 dirToPlayer)
{
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 pos = this->getWorldPos();

	pos.y = 1.3f;

	//Player to close detect!
	if (playerDist < 1.2f)
		return 1.0f;

	if (glm::dot(dirToPlayer, glm::vec3(0.0f, 0.0f, 1.0f)) > _detectFov)
	{
		//Get light at position
		float playerLight = _player->getLightAtPosition();
		
		//Account light for Anti-L Grenade
		if (glm::length(playerPos - glm::vec3(_player->getGrenadeData()[0]._grenadePositionWhenLanded)) < _player->getGrenadeData()[0].expanding)
			playerLight *= _player->getGrenadeData()[0].fading;

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
