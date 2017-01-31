#include "Character.h"

void Character::setCamera(Camera* camera)
{
    _camera = camera;
    _camera->setY(0.8);
    _velocity = glm::vec3(0,0,0);
}
void Character::onUpdate(float dt)
{
    _camera->setRX(rotation.x);
    _camera->setRY(rotation.y);
    _camera->moveX(_velocity.y * dt * sin(rotation.x));
    _camera->moveZ(_velocity.y * dt * cos(rotation.x));
    _camera->moveX(_velocity.x * dt * cos(rotation.x));
    _camera->moveZ(_velocity.x * dt * -sin(rotation.x));

}
void Character::onRender()
{

}
void Character::moveCharacter(const KeyboardEvent* event)
{
    if (event->getKey() == GLFW_KEY_W)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.y -= 1.0f;
        }
        else if (event->getAction() == GLFW_RELEASE)
        {
            _velocity.y += 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_A)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x -= 1.0f;
        }
        else if (event->getAction() == GLFW_RELEASE)
        {
            _velocity.x += 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_S)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.y += 1.0f;
        }
        else if (event->getAction() == GLFW_RELEASE)
        {
            _velocity.y -= 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_D)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x += 1.0f;
        }
        else if (event->getAction() == GLFW_RELEASE)
        {
            _velocity.x -= 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_T)
    {
        if (event->getAction() == GLFW_PRESS)
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
            _eventManager->execute(new cursorModeChangeEvent(_cursorMode));
        }
    }
}
void Character::moveMouse(const MouseMoveEvent* event)
{
    if (_hasMoved == false)
    {
        _hasMoved = true;
        _lastCursorPos = event->getPos();
        return;
    }
    glm::vec2 currentCurserPos = event->getPos();
    glm::vec2 deltaPos = currentCurserPos - _lastCursorPos;
    _lastCursorPos = currentCurserPos;
    if (_cursorMode == GLFW_CURSOR_DISABLED)
    {
        rotateY(deltaPos.y * -RotationSpeed);
        rotateX(deltaPos.x * -RotationSpeed);
    }
}
void Character::collectLoot(const CollectLootEvent* event)
{
    std::cout << "recieved loot of value: " << event->getValue() << std::endl;
    _lootValue += event->getValue();
}
Character::Character(EventManager *manager) : _eventManager(manager)
{
    _eventManager->listen(this, &Character::moveCharacter);
    _eventManager->listen(this, &Character::moveMouse);
    _eventManager->listen(this, &Character::collectLoot);
}
Character::Character()
{
}
Character::~Character()
{

}
