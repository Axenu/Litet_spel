#include "Game/Objects/Character.h"
#include "StaticVars.h"
#include "Game/Objects/CharacterEvents.h"

void Character::setCamera(Camera* camera)
{
    _camera = camera;
    _velocity = glm::vec3(0,0,0);
    _direction = glm::vec3(0,0,0);
    _speed = 2;
    _isMoving = 0;
}
void Character::onUpdate(float dt)
{
	move(dt);
	GridSquare newSquare = _currentLevel->operator[](glm::vec3(getWorldPos()));
	//Character moved on a square
	if (newSquare._square != _gridSquare._square) {
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

void Character::onRender()
{

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
				int points = _currentScene->loot(*_camera, 2);
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
        rotateY(deltaPos.y * -RotationSpeed);
        rotateX(deltaPos.x * -RotationSpeed);
        if (getRY() > glm::pi<float>()*0.5f)
        {
            setRY(glm::pi<float>()*0.5f);
        }
        if (getRY() < glm::pi<float>()*-0.5f)
        {
            setRY(glm::pi<float>()*-0.5f);
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
    _eventManager->listen(this, &Character::moveCharacter);
    _eventManager->listen(this, &Character::moveMouse);
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
