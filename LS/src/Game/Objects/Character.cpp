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
		testClimb();
	}

	GridSquare newSquare = _currentLevel->operator[](glm::vec3(getWorldPos()));
	//Character moved on a square
	if (newSquare._square != _gridSquare._square) {
		//Set new height
		if (!_climbing)
		{
			glm::vec3 pos = getWorldPos();
			setPositionY(_currentLevel->getGridHeight(pos));
		}
		//Send event
		CharacterSquareEvent squareEvent(this, newSquare);
		_eventManager->execute(squareEvent);
		this->_gridSquare = newSquare;
	}
}

void Character::move(float dt) {

	if (_moveDir.x != 0 || _moveDir.y != 0)
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
		//Add the movement velocity travel
		
		glm::vec3 forw2D = getForward();
		forw2D = glm::normalize(glm::vec3(forw2D.x, 0.f, forw2D.z));
		glm::vec3 right2D = getRight();
		right2D.y = 0;
		right2D = glm::normalize(right2D);
		_velocity = _moveDir.x * right2D * _isMoving * _speed;
		_velocity += _moveDir.y * forw2D * _isMoving * _speed;
	}
	else
	{
		_velocity = glm::vec3(0, 0, 0);
		_isMoving = 0;
	}
	//Calculate new camera position and update the camera
	_position = _currentLevel->wallCollission(_position, _velocity * dt);
}

void Character::climb(float dT)
{
	_animTime += dT;
	if (_animTime < _animEndTime)
	{

		if (_animTime < _animFirstPhaseTime)
		{
			glm::vec3 dir = _animEndPos - _position;
			glm::vec2 xzDir(dir.x, dir.z);
			xzDir = glm::normalize(xzDir);
			xzDir *= _speed * dT;
			Node::moveX(xzDir.x);
			Node::moveZ(xzDir.y);
		}
		else if (_animTime < _animSecondPhaseTime) //Animate climb phase
		{
			float yDist = _animEndPos.y - _position.y;
			float timeDiff = _animSecondPhaseTime - _animTime;
			if (timeDiff > 0.00001) //Check so animation phase is not about to end.
			{
				float yPos = dT * yDist / timeDiff;
				moveY(yPos);
			}
			else
			{
				setPositionY(_animEndPos.y);
			}
		}
		else //Animate end phase
		{
			glm::vec3 dir = _animEndPos - _position;
			glm::vec2 xzDir(dir.x, dir.z);
			xzDir = glm::normalize(xzDir);
			xzDir *= _speed * dT;
			Node::moveX(xzDir.x);
			Node::moveZ(xzDir.y);
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
		if (_canClimb)
		{
		//Calc length between destination pos and current pos in xz-plane
		float xzDist = glm::length(glm::vec2(_animEndPos.x, _animEndPos.z) - glm::vec2(_position.x, _position.z));
		//calc animation time
		_animFirstPhaseTime = xzDist / (2 * _speed);
		_animSecondPhaseTime = _animFirstPhaseTime + _heightDiff / _speed;
		_animEndTime = (_heightDiff + xzDist) / _speed;
		_animTime = 0.0f;
		_climbing = true;
		}
	}
}

void Character::testClimb()
{
	if (!_climbing)
	{
		_animEndPos = glm::vec3(getWorldPos());
		glm::vec3 dir = glm::vec3(_currentScene->getCamera().getLookAt());
		if (_currentLevel->testForClimb(_animEndPos, dir, _heightDiff))
		{
			if (!_canClimb)
			{
				_canClimb = true;
				CanClimbEvent cEvent(_canClimb);
				_eventManager->execute(cEvent);
			}
		}
		else
		{
			if (_canClimb)
			{
				_canClimb = false;
				CanClimbEvent cEvent(_canClimb);
				_eventManager->execute(cEvent);
			}
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
            _moveDir.y += 1.0f;
        else if (event.getAction() == GLFW_RELEASE)
			_moveDir.y -= 1.0f;
    }
    else if (event.getKey() == GLFW_KEY_A)
    {
        if (event.getAction() == GLFW_PRESS)
			_moveDir.x += 1.0f;
        else if (event.getAction() == GLFW_RELEASE)
			_moveDir.x -= 1.0f;
    }
    else if (event.getKey() == GLFW_KEY_S)
    {
        if (event.getAction() == GLFW_PRESS)
			_moveDir.y -= 1.0f;
        else if (event.getAction() == GLFW_RELEASE)
			_moveDir.y += 1.0f;
    }
    else if (event.getKey() == GLFW_KEY_D)
    {
        if (event.getAction() == GLFW_PRESS)
			_moveDir.x -= 1.0f;
        else if (event.getAction() == GLFW_RELEASE)
			_moveDir.x += 1.0f;
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

glm::vec3 Character::getEyePos()
{
	return _currentScene->getCamera().getWorldPos();
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
		float tilt = deltaPos.y * RotationSpeed;
		_camTilt += tilt;
        rotateY(deltaPos.x * -RotationSpeed);
		if (_camTilt > glm::pi<float>()*0.5f)
			_camTilt = glm::pi<float>()*0.5f;
		else if (_camTilt < glm::pi<float>()*-0.5f)
			_camTilt = glm::pi<float>()*-0.5f;
		else //Rotate
			_currentScene->getCamera().rotateX(tilt);
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
	_camTilt = 0;
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
	_camTilt = 0;
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
