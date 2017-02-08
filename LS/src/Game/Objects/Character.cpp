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
	glm::ivec2 newSquare = _currentLevel->getSquare(getWorldPos());
	gridType grid = _currentLevel->operator[](newSquare);
	//Character moved on a square
	if (newSquare != _gridSquare) {
		CharacterSquareEvent squareEvent(this, newSquare, grid);
		_eventManager->execute(squareEvent);
		this->_gridSquare = newSquare;
	}
	if (grid == gridType::exiting)
		sic::CloseWindow = true;
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

#pragma region Events

void Character::doYouWantToWin(const KeyboardEvent & event)
{
	if (event.getKey() == GLFW_KEY_G)
	{
		if (event.getAction() == GLFW_PRESS)
		{
			buttonpressed = true;
		}
		else if (event.getAction() == GLFW_RELEASE)
		{
			buttonpressed = false;
		}
	}
}
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
    else if (event.getKey() == GLFW_KEY_T)
    {
        if (event.getAction() == GLFW_PRESS)
        {
            if (_cursorMode == GLFW_CURSOR_NORMAL)
            {
                _cursorMode = GLFW_CURSOR_DISABLED;
            }
            else
            {
                _cursorMode = GLFW_CURSOR_NORMAL;
            }
            _hasMoved = false;
            cursorModeChangeEvent event(_cursorMode);
            _eventManager->execute(event);
        }
    }
	else if (event.getKey() == GLFW_KEY_E)
	{
		int points = _currentScene->loot(*_camera, 2);
		if (points > 0)
		{
			if (event.getAction() == GLFW_PRESS)
			{
				CollectLootEvent event(points);
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
    }
    if (getRY() > glm::pi<float>()*0.5f)
    {
        setRY(glm::pi<float>()*0.5f);
    }
    if (getRY() < glm::pi<float>()*-0.5f)
    {
        setRY(glm::pi<float>()*-0.5f);
    }
}
void Character::collectLoot(const CollectLootEvent& event)
{
    std::cout << "recieved loot of value: " << event.getValue() << std::endl;
    _lootValue += event.getValue();
}

#pragma endregion

#pragma region Set & Construction 

void Character::setLevel(Grid *level)
{
	this->_currentLevel = level;
	this->_gridSquare = level->getSquare(getWorldPos());
}
void Character::setScene(Scene * scene)
{
	_currentScene = scene;
}
Character::Character(glm::vec3 pos, EventManager *manager) :
	GameObject(), _eventManager(manager)
{
	setPosition(pos);
    _eventManager->listen(this, &Character::moveCharacter);
	_eventManager->listen(this,&Character::doYouWantToWin);
    _eventManager->listen(this, &Character::moveMouse);
    _eventManager->listen(this, &Character::collectLoot);
	buttonpressed = false;
	charactermovedoutsidebox = false;
}
Character::Character()
{
}
Character::~Character()
{

}

#pragma endregion