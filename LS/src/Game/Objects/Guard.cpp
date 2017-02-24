#include "Game/Objects/Guard.h"
#include "math/MathFunctions.h"



void Guard::setLevel(Level *level)
{
	this->_currentLevel = level;
}

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
	
	//face(_player->getWorldPos());
	GameObject::update(dt); //Let object update the move vars before doing our update logic

	if (glm::length(this->getWorldPos() - _player->getWorldPos()) < GUARDVIEWDISTANCE)
	{
		if (DetectedPlayer())
		{
			//std::cout << "Detected player!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			GameOverEvent event(false);
			_eventManager->execute(event);

			/*glm::vec3 detP = _player->getWorldPos();
			std::cout << "Detected\n";
			std::cout << "Guard Co: "<< pos.x << ", " << pos.y << ", " << pos.z << '\n';
			std::cout << "Player Co: " << detP.x << ", " << detP.y << ", " << detP.z << '\n';*/
		}
	}

}

glm::vec2 Guard::getNextPosition()
{
	_whatPathToLoad += 1;
	if (_whatPathToLoad > sizeOfVector - 1)
		_whatPathToLoad = 0;
	return(walkingPointsen[_whatPathToLoad]);

//	glm::vec2 test = (*walkingPoints)[_whatPathToLoad];
//	return (*walkingPoints)[_whatPathToLoad];
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
}

Guard::~Guard()
{
}

bool Guard::DetectedPlayer()
{

	glm::vec3 pos = this->getWorldPos();
	glm::vec3 playerPos = _player->getWorldPos();
	glm::vec3 playerEyePos = _player->getEyePos();

	pos.y = 1.3f;

	glm::vec3 dirToPlayer = playerPos - pos;
	float playerDist = glm::length(dirToPlayer);
	dirToPlayer = glm::normalize(dirToPlayer);


	//Player to close detect!
	if (playerDist < 1.2f)
		return true;

	if (glm::dot(dirToPlayer, getForward()) > _detectFov)
	{
		float playerLight = 1.0f;
		//Calculate light at position
		playerLight = glm::min(_currentLevel->calcLightOnPosition(playerPos), 1.0f);
		//Account light for Anti-L Grenade
		if (glm::length(playerPos - glm::vec3(_player->getGrenadeData()._grenadePositionWhenLanded)) < _player->getGrenadeData().expanding)
			playerLight *= _player->getGrenadeData().fading;

		//Account for the fact if the player is right infront of the guard or not
		playerLight *= glm::dot(getForward(), dirToPlayer);

		//Distance to stuff
		float wallDist = _currentLevel->getGrid().getDist(pos, dirToPlayer, GUARDVIEWDISTANCE * playerLight);
		if (playerDist > wallDist)
		{
			return false;
		}

		bool obscured = true;
		float objectDist = _currentLevel->getGrid().getDist(pos, dirToPlayer, GUARDVIEWDISTANCE * playerLight, _player->getEyePos(), object, obscured);
		if (playerDist < objectDist || (playerDist > objectDist && obscured == false))
			return true;
		 
	}
	return false;
}