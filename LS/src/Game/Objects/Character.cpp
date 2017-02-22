#include "Game/Objects/Character.h"
#include "StaticVars.h"
#include "Game/Objects/CharacterEvents.h"

void Character::onUpdate(float dt)
{
	if (_climbing)
	{
		climb(dt);
	}
	else
	{
		move(dt);
	}

	GridSquare newSquare = _currentLevel->operator[](glm::vec3(getWorldPos()));
	//Character moved on a square
	if (newSquare._square != _gridSquare._square) {
		//Set new height
		if (!_climbing)
		{
			glm::vec3 pos = getWorldPos();
			setY(_currentLevel->getGridHeight(pos));
		}
		//Send event
		CharacterSquareEvent squareEvent(this, newSquare);
		_eventManager->execute(squareEvent);
		this->_gridSquare = newSquare;
	}
}

void Character::move(float dt) {

	if (_direction.x != 0 || _direction.y != 0)
	{
		//calculate inertia
		if (_isMoving < 1)
		{
			_isMoving += dt * 8;
			if (_isMoving > 1)
			{
				_isMoving = 1;
			}
		}
		_velocity = glm::normalize(_direction) * _isMoving * _speed;
	}
	else
	{
		_velocity = glm::vec3(0, 0, 0);
		_isMoving = 0;
	}
	glm::vec3 actualVelocity;
	//Calculate the velocity
	actualVelocity.x = _velocity.y * dt * sinf(_rotation.x);
	actualVelocity.x += _velocity.x * dt * cosf(_rotation.x);
	actualVelocity.y = _velocity.y * dt * cosf(_rotation.x);
	actualVelocity.y += _velocity.x * dt * -sinf(_rotation.x);

	//Calculate new camera position and update the camera
	_currentLevel->wallCollission(&_position, actualVelocity);
}

void Character::climb(float dT)
{
	_animTime += dT;
	if (_animTime < _animEndTime)
	{
		float firstPhaseTime = 0.5f;
		float secondPhaseTime = _animEndTime - 0.5f;
		if (_animTime < firstPhaseTime)
		{
			float timeDiff = _animEndTime - _animTime;
			if (abs(timeDiff) > 0.00001) //Check if animation phase is not about to end.
			{
				glm::vec3 dir = _animEndPos - _position;
				dir *= dT / timeDiff;
				Node::moveX(dir.x);
				Node::moveZ(dir.z);
			}
		}
		else if (_animTime < secondPhaseTime) //Animate climb phase
		{
			float yDist = _animEndPos.y - _position.y;
			float timeDiff = secondPhaseTime - _animTime;
			if (timeDiff > 0.00001) //Check if animation phase is not about to end.
			{
				float yPos = dT * yDist / timeDiff;
				moveY(yPos);
			}
			else
			{
				setY(_animEndPos.y);
			}
		}
		else //Animate end phase
		{
			float timeDiff = _animEndTime - _animTime;
			if (abs(timeDiff) > 0.00001) //Check if animation phase is not about to end.
			{
				glm::vec3 dir = _animEndPos - _position;
				dir *= dT / timeDiff;
				Node::move(dir);
			}
		}
	}
	else
	{
		setPosition(_animEndPos);
		_climbing = false;
	}
}

void Character::tryClimb()
{
	if (!_climbing)
	{
		_animEndPos = glm::vec3(getWorldPos());
		glm::vec3 dir = glm::vec3(_currentScene->getCamera().getLookAt());
		_currentLevel->testForClimb(_animEndPos, dir,_animEndTime);
		if (_animEndTime > 0.0f)
		{
			_animTime = 0.0f;
			_climbing = true;
		}
	}
}

void Character::onRender()
{
	
}

GrenadeValues Character::getGrenadeData()
{
//	std::cout << "antilightGrenade" << _antiLightGrenade->getgrenadePositionWhenlanded().x << "," << _antiLightGrenade->getgrenadePositionWhenlanded().y << "," << _antiLightGrenade->getgrenadePositionWhenlanded().z << std::endl;

	return _antiLightGrenade->getgrenadeData();
}

int* Character::getLootValuePointer()
{
    return &_lootValue;
}

#pragma region Events

void Character::moveCharacter(const KeyboardEvent& event)
{
    if (event.getKey() == GLFW_KEY_W)
    {
        if (event.getAction() == GLFW_PRESS)
        {
            _direction.y -= 1.0f;
        }
        else if (event.getAction() == GLFW_RELEASE)
        {
            _direction.y += 1.0f;
        }
    }
    else if (event.getKey() == GLFW_KEY_A)
    {
        if (event.getAction() == GLFW_PRESS)
        {
            _direction.x -= 1.0f;
        }
        else if (event.getAction() == GLFW_RELEASE)
        {
            _direction.x += 1.0f;
        }
    }
    else if (event.getKey() == GLFW_KEY_S)
    {
        if (event.getAction() == GLFW_PRESS)
        {
            _direction.y += 1.0f;
        }
        else if (event.getAction() == GLFW_RELEASE)
        {
            _direction.y -= 1.0f;
        }
    }
    else if (event.getKey() == GLFW_KEY_D)
    {
        if (event.getAction() == GLFW_PRESS)
        {
            _direction.x += 1.0f;
        }
        else if (event.getAction() == GLFW_RELEASE)
        {
            _direction.x -= 1.0f;
        }
    }
	else if (event.getKey() == GLFW_KEY_E)
	{
			if (event.getAction() == GLFW_PRESS)
			{
				int points = _currentScene->loot(2);
				if (points > 0)
				{
					CollectLootEvent event(points);
					_lootValue += points;
					_eventManager->execute(event);
				}
			}
	}
    else if (event.getKey() == GLFW_KEY_G)
	{
        if (event.getAction() == GLFW_PRESS)
        {
            if (_gridSquare._grid == gridType::exiting) // && _hasVictoryLoot TODO
            {
                //call endGameEvent
                GameOverEvent event(true);
                _eventManager->execute(event);
            }
        }
	}
	else if (event.getKey() == GLFW_KEY_Q)
	{
		if (event.getAction() == GLFW_PRESS)
		{
		//	std::cout << this->getWorldPos().x << this->getWorldPos().y << this->getWorldPos().z << std::endl;
			_antiLightGrenade->ThrowTheLightgrenade(this->getWorldPos(), _currentScene->getCamera().getLookAt());
		}
	}
	else if (event.getKey() == GLFW_KEY_LEFT_CONTROL)
	{

		if (event.getAction() == GLFW_PRESS)
		{
			_currentScene->getCamera().moveY(-0.5);
			_speed = _speed - 1;
		}
		if (event.getAction() == GLFW_RELEASE)
		{
			_currentScene->getCamera().moveY(0.5);
			_speed = _speed + 1;
		}

	}
	else if (event.getKey() == GLFW_KEY_SPACE)
	{
		tryClimb();
	}
}
void Character::moveMouse(const MouseMoveEvent& event)
{
    if (_hasMoved == false)
    {
        _hasMoved = true;
        _lastCursorPos = event.getPos();
        return;
    }
    glm::vec2 currentCurserPos = event.getPos();
    glm::vec2 deltaPos = currentCurserPos - _lastCursorPos;
    _lastCursorPos = currentCurserPos;
    if (_cursorMode == GLFW_CURSOR_DISABLED)
    {	
		//Player rotats in the walking plane.
		rotateX(deltaPos.x * -RotationSpeed);
		//Camera rotates in up and down.                                                                                      
		Camera* cam = &_currentScene->getCamera();  
		cam->rotateY(deltaPos.y * -RotationSpeed);
        if (cam->getRY() > glm::pi<float>()*0.5f)
        {
            cam->setRY(glm::pi<float>()*0.5f);
        }
        if (cam->getRY() < glm::pi<float>()*-0.5f)
        {
            cam->setRY(glm::pi<float>()*-0.5f);
        }
    }
}

#pragma endregion

#pragma region Set & Construction

void Character::setLevel(Grid *level)
{
	this->_currentLevel = level;
	this->_gridSquare = level->operator[](glm::vec3(getWorldPos()));
}
void Character::setScene(Scene * scene)
{
	_currentScene = scene;
}

Character::Character(glm::vec3 pos, EventManager *manager) :
	GameObject(), _eventManager(manager)
{
    _lootValue = 0;
	setPosition(pos);
	_velocity = glm::vec3(0, 0, 0);
	_direction = glm::vec3(0, 0, 0);
	_speed = 2;
	_isMoving = 0;
    _eventManager->listen(this, &Character::moveCharacter);
    _eventManager->listen(this, &Character::moveMouse);
}

Character::Character(glm::vec3 pos, EventManager *manager,AntiLightGrenade * grenade) :
	GameObject(), _eventManager(manager)
{
	_lootValue = 0;
	setPosition(pos);
	_velocity = glm::vec3(0, 0, 0);
	_direction = glm::vec3(0, 0, 0);
	_speed = 2;
	_isMoving = 0;
	_eventManager->listen(this, &Character::moveCharacter);
	_eventManager->listen(this, &Character::moveMouse);
	_antiLightGrenade = grenade;
}

Character::Character()
{
    _lootValue = 0;
}
Character::~Character()
{
    _eventManager->unlisten(this, &Character::moveCharacter);
    _eventManager->unlisten(this, &Character::moveMouse);

}

#pragma endregion
