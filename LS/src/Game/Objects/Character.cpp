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

	_timerForGrenade += dt;
	if (_timerForGrenade > 10)
	{
		_timerForGrenade = 6;
	}
	if (_timerForGrenade < 2 && noMoreGrenadeCount)
		LightGrenadeClock = 2 - _timerForGrenade;
	else
		LightGrenadeClock = 0;

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

int Character::getGrenadeID()
{
	return _grenadeID;
}

float Character::calcLightOnPosition()
{
	float wallDist = 0.0f;
	glm::vec4 posColor(0.0f);
	glm::vec3 pos(this->getWorldPos());
	AABB playerBox(pos, 0.5f);
	std::vector<PointLightObject*> lights = _currentScene->fetchDynamicObjects<PointLightObject>(playerBox);

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		glm::vec3 lightRay = lights[i]->getLightInfo()._pos - pos;

		wallDist = _currentLevel->getDist(lights[i]->getLightInfo()._pos, lightRay, lights[i]->getLightInfo()._fadeDist);

		if (wallDist > glm::length(lightRay))
		{
			lightRay = glm::normalize(lightRay);
			float diff = glm::max(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), lightRay), 0.0f);
			float distance = glm::length(lightRay);
			float att = glm::max(1.0f - (distance / lights[i]->getLightInfo()._fadeDist), 0.0f);

			posColor += lights[i]->getLightInfo()._diffuse * diff * att;
		}
	}

	return glm::min(posColor.x + posColor.y + posColor.z + 0.5f, 1.0f);
}


void Character::onRender()
{

}

std::vector<GrenadeValues> Character::getGrenadeData()
{
//	std::cout << "antilightGrenade" << _antiLightGrenade->getgrenadePositionWhenlanded().x << "," << _antiLightGrenade->getgrenadePositionWhenlanded().y << "," << _antiLightGrenade->getgrenadePositionWhenlanded().z << std::endl;
	std::vector<GrenadeValues> _grenadevalues;
	_grenadevalues.clear();
	for (int i = 0; i < _antiLightGrenade.size(); i++)
		_grenadevalues.push_back( _antiLightGrenade[i]->getgrenadeData());
	return _grenadevalues;
}

int* Character::getLootValuePointer()
{
    return &_lootValue;
}

int* Character::getGrenadeCountPointer()
{
	return &_lightGrenadeCount;
}

float * Character::getGrenadeCooldownTimer()
{
	return &LightGrenadeClock;
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
			if (_timerForGrenade > 2)
			{
			_antiLightGrenade[_grenadeCount]->ThrowTheLightgrenade(this->getWorldPos(), _currentScene->getCamera().getLookAt());
			_timerForGrenade = 0;
			if(_lightGrenadeCount>0)
			_lightGrenadeCount--;
			_grenadeID++;
			_grenadeCount++;
			if (_grenadeCount == _antiLightGrenade.size())
			{
			_grenadeCount = 0;
			noMoreGrenadeCount = false;
			}
			}
			
		}
	}
	else if (event.getKey() == GLFW_KEY_LEFT_CONTROL)
	{

		if (event.getAction() == GLFW_PRESS)
		{
			if(sneaking == true)
			{
				_currentScene->getCamera().moveY(0.5);
				_speed = _speed + 1;
				sneaking = false;
			}
			else
			{

				_currentScene->getCamera().moveY(-0.5);
				_speed = _speed - 1;
				sneaking = true;
			}
		}
	}
	else if (event.getKey() == GLFW_KEY_SPACE)
	{
		tryClimb();
	}
	else if (event.getKey() == GLFW_KEY_T)
	{
		std::cout << "X: " << getEyePos().x << " Y: " << getEyePos().z << std::endl;
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

int Character::amountOfGrenades()
{
	return _antiLightGrenade.size();
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

Character::Character(glm::vec3 pos, EventManager *manager,std::vector<AntiLightGrenade*> grenade) :
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
	_climbing = false;
	_canClimb = false;
	_animFirstPhaseTime = 0.0f;
	_animSecondPhaseTime = 0.0f;
	_animEndPos = glm::vec3(0.0f);
	_animTime = 0.0f;
	_animEndTime = 0.0f;
	_heightDiff = 0.0f;
	_antiLightGrenade = grenade;
	_lightGrenadeCount = GrenadeAmountFromCharacter;
	_grenadeID = -1;
	_timerForGrenade = 0;
	noMoreGrenadeCount = true;
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
