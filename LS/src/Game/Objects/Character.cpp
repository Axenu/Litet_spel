#include "Game/Objects/Character.h"
#include "StaticVars.h"
#include "Game/Objects/CharacterEvents.h"
#include "Event/SceneEvents/CreateEvents.h"
#include "Game/Objects/Guard.h"

void Character::onUpdate(float dt)
{
	switch (_state)
	{
	case CharState::guardVision:
		gVisionTimerUpdate(dt);
		break;
	case CharState::climbing:
		climb(dt);
		break;
	default:
		move(dt);
		testClimb();
		break;
	}

	GridSquare newSquare = _currentLevel->operator[](glm::vec3(getWorldPos()));
	//Character moved on a square
	if (newSquare._square != _gridSquare._square) {
		//Set new height
		//if (_state != CharState::climbing)
		//{
		//	glm::vec3 pos = getWorldPos();
		//	setPositionY(_currentLevel->getGridHeight(pos));
		//}
		//Send event
		CharacterSquareEvent squareEvent(this, newSquare);
		_eventManager->execute(squareEvent);
		this->_gridSquare = newSquare;
	}

	_grenadeTimer -= dt;
	if (_grenadeTimer <= 0 || _lightGrenadeCount == 0)
		_grenadeTimer = 0.f;

	_lightAtPos = calcLightAtPosition();
	calcNoise();

	//calculate score based on detection
	if (_detectionLevel > 0.0f)
	{
		_score -= _detectionLevel * dt;
		_detectionLevel = 0.0f;
		_score = std::max(0.0f, _score);
	}
	//update sound listener position
	SoundManager::getInstance().setListenerPosition(this->getWorldPos(), this->getForward(), this->getUp(), glm::vec3(0,0,0));

	if (!canLean() && _lean != 0)
	{
		rotateZ(_rotate * -1.0f);
		_lean = 0;
	}


}

void Character::init()
{
	GameObject::init();
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
		_walkingSound->play();

	}
	else
	{
		_velocity = glm::vec3(0, 0, 0);
		_isMoving = 0;
		_walkingSound->pause();
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
			if (timeDiff > 0.00001f) //Check so animation phase is not about to end.
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
		_state = CharState::normal;
		_climbSound->pause();
	}
}

void Character::tryClimb()
{
	if (_state != CharState::climbing)
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
			_state = CharState::climbing;
			_climbSound->setPlayPosition(1000);
			_climbSound->play();

		}
	}
}

void Character::testClimb()
{
	if (_state != CharState::climbing)
	{
		_animEndPos = glm::vec3(getWorldPos());
		glm::vec3 dir = glm::vec3(_currentScene->getCamera().getLookAt());
		if (_currentLevel->testForClimb(_animEndPos, dir, _heightDiff, _height))
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

bool Character::guardVision()
{
	float objectDist = 10.f;
	//Check distance to walls of the map
	_currentLevel->getDist(_currentScene->getCamera().getWorldPos(), _currentScene->getCamera().getLookAt(), objectDist, objectDist, gridType::wall);
	//Get closest pick object
	Guard* gPtr = dynamic_cast<Guard*>(_currentScene->pick(objectDist, _currentScene->getCamera().getParent()));
	if (gPtr)
	{
		_currentScene->getCamera().setParent(gPtr);
		_currentScene->getCamera().setPosition(glm::vec3(0.0f, 1.4f, 0.12f));
		if (_state == CharState::normal)
		{
			_guardVisDuraTimer = 5.0f;
			//vcall event
			GuardVisionEvent event(true, _guardVisDuraTimer * 0.2f);
			_eventManager->execute(event);
		}
		_state = CharState::guardVision;
		return true;
	}
	else
	{
		return false;
	}
}

void Character::returnVision()
{
	_currentScene->getCamera().setParent(this);
	//Setting camera to characters height(if sneaking lowering �t to sneaking height)
	_currentScene->getCamera().setPositionY(_sneaking ? _height - SneakDiff : _height);
	//resets the cameras forwardvec and its tilt
	_currentScene->getCamera().setForward(glm::vec3(0.0f, 0.0f, 1.0f));
	_camTilt = glm::bvec2(0.0f);
	_state = CharState::normal;
	GuardVisionEvent event(false, 0.0f);
	_eventManager->execute(event);
}

bool Character::canLean()
{
	if (_currentLevel->getDist(this->getEyePos(), getForward(), 0.2f, wall) || _currentLevel->getDist(this->getEyePos(), -1.0f * getForward(), 0.2f, wall))
		return false;

	if (_currentLevel->getDist(this->getEyePos(), this->getRight(), 0.3f, wall) || _currentLevel->getDist(this->getEyePos(), -1.0f * this->getRight(), 0.3f, wall))
		return false;

	if (_currentLevel->getDist(this->getEyePos(), getForward(), 0.2f, this->getEyePos(), object) || _currentLevel->getDist(this->getEyePos(), -1.0f * getForward(), 0.2f, this->getEyePos(), object))
		return false;

	if (_currentLevel->getDist(this->getEyePos(), this->getRight(), 0.3f, this->getEyePos(), object) || _currentLevel->getDist(this->getEyePos(), -1.0f * this->getRight(), 0.3f, this->getEyePos(), object))
		return false;

	return true;
}

float Character::getHeight()
{
	if (_sneaking)
		return _height - SneakDiff;

	return _height;
}

void Character::gVisionTimerUpdate(float dt)
{
	_guardVisDuraTimer -= dt;
	GuardVisionEvent event(true, _guardVisDuraTimer * 0.2f);
	_eventManager->execute(event);
	if (_guardVisDuraTimer < 0.0f)
		returnVision();
}


float Character::calcLightAtPosition()
{
	glm::vec3 pos(this->getWorldPos());
	AABB playerBox(pos, 0.5f);
	std::vector<PointLightObject*> lights = _currentScene->fetchDynamicObjects<PointLightObject>(playerBox);
	float intensity = 0.0f;

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		glm::vec2 lr(lights[i]->getLightInfo()._pos.x - pos.x, lights[i]->getLightInfo()._pos.z - pos.z);
		if (glm::length(lr) < lights[i]->getLightInfo()._fadeDist)
		{
			intensity += std::max(1.0f - (glm::length(lr) / lights[i]->getLightInfo()._fadeDist), 0.0f); //Shader falloff algorithm
		}
	}
	intensity = std::max(intensity, 0.2f); //ambient
	intensity = std::min(intensity, 1.0f); //Cant be too bright

	std::vector<AntiLightGrenade*> nades = _currentScene->fetchDynamicObjects<AntiLightGrenade>(playerBox);
	for (unsigned int i = 0; i < nades.size(); i++)
	{
		GrenadeValues val = nades[i]->getgrenadeData();
		//Account light for Anti-L Grenade
		if (glm::length(pos - glm::vec3(nades[i]->getWorldPos())) < val.expanding)
			intensity *= val.fading;
	}
	return intensity;
}

float Character::getLightAtPosition()
{
	return _lightAtPos;
}

void Character::calcNoise()
{
	switch (_state)
	{
	case CharState::climbing:
		_movmentNoise = CLIMBINGNOISE;
		if (_climbSound)
			_climbSound->setVolume(1.0f);
		break;
	case CharState::guardVision:
		_movmentNoise = 0.0f;
		break;
	default:
		_movmentNoise = WALKINGNOISE;
		if (_walkingSound)
			_walkingSound->setVolume(1.0f);
		_movmentNoise *= _isMoving;
		break;
	}
	if (_sneaking)
	{
		_movmentNoise *= SNEAKINGMODIFIER;
		if (_walkingSound)
			_walkingSound->setVolume(SNEAKINGMODIFIER);
		if (_climbSound)
			_climbSound->setVolume(SNEAKINGMODIFIER);
	}
}

float Character::getNoise()
{
	return _movmentNoise;
}

void Character::onRender()
{

}

int* Character::getLootValuePointer()
{
	return &_lootValue;
}

float* Character::getScoreValuePointer()
{
    return &_score;
}

int* Character::getGrenadeCountPointer()
{
	return &_lightGrenadeCount;
}

float * Character::getGrenadeCooldownTimer()
{
	return &_grenadeTimer;
}

void Character::detected(const GuardAlertEvent &event)
{
	_detectionLevel += event._detection;
}

void Character::moveCharacter(const KeyboardEvent& event)
{
	switch (_state)
	{
	case CharState::guardVision:
		guardVisionKeyInput(event);
		break;
	default:
		normalKeyInput(event);
		break;
	}
}

glm::vec3 Character::getEyePos()
{
	return _currentScene->getCamera().getWorldPos();
}

glm::vec3 Character::getVelocity()
{
	return _velocity;
}

#pragma region Events

void Character::normalKeyInput(const KeyboardEvent & event)
{
	if (charMovement(event)) {} // CharMovements takes care of WASD-input

	else if (event.getKey() == EXIT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			//exit game
			if (_gridSquare._grid == gridType::exiting && _lootValue >= MAX_LOOT_LEVEL * 0.5f)
			{
				//call endGameEvent
				GameOverEvent event(true);
				_eventManager->execute(event);
				Sound *tempSound = SoundManager::getInstance().play2DSound(WIN_SOUND, false, false);
				tempSound->setVolume(0.2f);
				tempSound->play();
			}
		}
	}
	else if (event.getKey() == ANTILIGHTGRENADE)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			if (_lightGrenadeCount > 0 && _grenadeTimer <= 0)
			{
				CreateLightGrenade e("grenade.obj", this->getEyePos(), _currentScene->getCamera().getLookAt());
				_eventManager->execute(e);
				_grenadeTimer = _grenadeCooldown;
				_lightGrenadeCount--;
			}

		}
	}
	else if (event.getKey() == SNEAK)
	{

		if (event.getAction() == GLFW_PRESS)
		{
			if (_sneaking == true)
			{
				_currentScene->getCamera().moveY(SneakDiff);
				_speed = _speed + 1;
				_sneaking = false;
			}
			else
			{

				_currentScene->getCamera().moveY(-SneakDiff);
				_speed = _speed - 1;
				_sneaking = true;
			}
		}
	}
	else if (event.getKey() == CLIMB)
	{
		tryClimb();
	}
	else if (event.getKey() == GETEYEPOS)
	{
		std::cout << "X: " << getEyePos().x << " Y: " << getEyePos().z << std::endl;
	}
	else if (event.getKey() == VISION)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			guardVision();
		}
	}
	else if (event.getKey() == GETPOS)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			glm::ivec2 pos = _currentLevel->getSquare(this->getWorldPos());
			std::cout << "(" << pos.x << ", " << pos.y << ")" << std::endl;
		}
	}
}

void Character::guardVisionKeyInput(const KeyboardEvent & event)
{
	if (charMovement(event)) {} //Takes care so movement vectors keeps up to date.
	else if (event.getKey() == VISION)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			guardVision();
		}
	}
	else if (event.getKey() == RETURN)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			returnVision();
		}
	}
}

bool Character::charMovement(const KeyboardEvent& event)
{
	if (event.getKey() == MOVE_FORWARD)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_moveDir.y += 1.0f;
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			_moveDir.y -= 1.0f;
			return true;
		}
		return false;
	}
	else if (event.getKey() == MOVE_LEFT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_moveDir.x += 1.0f;
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			_moveDir.x -= 1.0f;
			return true;
		}
		return false;
	}
	else if (event.getKey() == MOVE_BACK)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_moveDir.y -= 1.0f;
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			_moveDir.y += 1.0f;
			return true;
		}
		return false;
	}
	else if (event.getKey() == MOVE_RIGHT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_moveDir.x -= 1.0f;
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			_moveDir.x += 1.0f;
			return true;
		}
		return false;
	}
	else if (event.getKey() == LEAN_LEFT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_lean--;
			_rotate = -1.0f * (M_PIf / 8.0f);
			rotateZ(_rotate);
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			if (_lean < 0)
			{
				_lean++;
				rotateZ(-1.0f * _rotate);
				return true;
			}
		}
	}
	else if (event.getKey() == LEAN_RIGHT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			_lean++;
			_rotate = (M_PIf / 8.0f);
			rotateZ(_rotate);
			return true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			if (_lean > 0)
			{
				_lean--;
				rotateZ(-1.0f * _rotate);
				return true;
			}
		}
	}
	return false;
}

/* Ensure rotation is within bounds
curRot << total rotation
amount << Frame rotation amount
max, min << Boundaries
*/
float maxRotation(float curRot, float amount, float max, float min)
{
	//Rotate only to max angle
	if (amount + curRot > max)
		amount = std::fmaxf(0.f, max - curRot);
	else if (amount + curRot < min)
		amount = std::fminf(0.f, min - curRot);
	return amount;
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
		// Left / Right camera rotation.
		float rotate = deltaPos.x * -RotationSpeed;
		switch (_state)
		{
		case CharState::guardVision:
			//Rotate
			rotate = maxRotation(_camTilt.x, rotate, glm::pi<float>() * MAXVIEWANGLE_SIDE, -glm::pi<float>() * MAXVIEWANGLE_SIDE);
			_camTilt.x += rotate;
			_currentScene->getCamera().rotateY(rotate);
			break;
		default:
			rotateY(rotate);
			break;
		}


		// Up / Down camera rotation.
		float tilt = deltaPos.y * RotationSpeed;
		tilt = maxRotation(_camTilt.y, tilt, glm::pi<float>() * MAXVIEWANGLEY, -glm::pi<float>() * MAXVIEWANGLEY);
		//Tilt
		_camTilt.y += tilt;
		_currentScene->getCamera().rotateX(tilt);
	}
}

void Character::clickMouse(const MouseClickEvent& event)
{
	if (event.getKey() == LOOT)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			int points = _currentScene->loot(2);
			if (points > 0)
			{
				CollectLootEvent event(points);
				_lootValue += points;
				_score += points;
				Sound *tempSound = SoundManager::getInstance().play2DSound(LOOTINGSOUND, false, true);
				tempSound->setPlayPosition(200);
				_eventManager->execute(event);
			}
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

void Character::pause()
{
	_eventManager->unlisten(this, &Character::moveCharacter);
	_eventManager->unlisten(this, &Character::moveMouse);
	_eventManager->unlisten(this, &Character::detected);
	_eventManager->unlisten(this, &Character::clickMouse);
}
void Character::resume()
{
	_eventManager->listen(this, &Character::moveCharacter);
	_eventManager->listen(this, &Character::moveMouse);
	_eventManager->listen(this, &Character::detected);
	_eventManager->listen(this, &Character::clickMouse);
	_hasMoved = false;
}
Character::Character(glm::vec3 pos, EventManager *manager, int grenadeCount, float height) :
	GameObject()
{
	_eventManager = manager;
	_state = CharState::normal;
	_height = height;
	_lootValue = 0;
	_score = 0.0f;
	setPosition(pos);
	_velocity = glm::vec3(0, 0, 0);
	_camTilt = glm::vec2(0.0f);
	_speed = 2;
	_isMoving = 0;
	_eventManager->listen(this, &Character::moveCharacter);
	_eventManager->listen(this, &Character::moveMouse);
	_eventManager->listen(this, &Character::detected);
	_eventManager->listen(this, &Character::clickMouse);
	_canClimb = false;
	_animFirstPhaseTime = 0.0f;
	_animSecondPhaseTime = 0.0f;
	_animEndPos = glm::vec3(0.0f);
	_animTime = 0.0f;
	_animEndTime = 0.0f;
	_heightDiff = 0.0f;
	_lightGrenadeCount = grenadeCount;
	_grenadeTimer = 0;
	_lightAtPos = 1.0f;
	_lean = false;
	_rotate = 0.0f;
	_walkingSound = SoundManager::getInstance().play2DSound(PLAYERWALKING, true, false);
	_climbSound = SoundManager::getInstance().play2DSound(CLIMBINGSOUND, false, false);
}

Character::Character()
{
	_lootValue = 0;
}
Character::~Character()
{
    _eventManager->unlisten(this, &Character::moveCharacter);
    _eventManager->unlisten(this, &Character::moveMouse);
	_eventManager->unlisten(this, &Character::detected);
	_eventManager->unlisten(this, &Character::clickMouse);
	_walkingSound->drop();
	_climbSound->drop();
}

#pragma endregion
