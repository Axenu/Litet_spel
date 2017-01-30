#include "Character.h"

void Character::setCamera(Camera* camera)
{
    _camera = camera;
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
    // std::cout << rotation.x << '\n';
    // std::cout << sin(rotation.x) << std::endl;

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
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.y += 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_A)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x -= 1.0f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.x += 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_S)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.y += 1.0f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.y -= 1.0f;
        }
    }
    else if (event->getKey() == GLFW_KEY_D)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x += 1.0f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.x -= 1.0f;
        }
    }
    //debug
    // std::cout << "x: " << _velocity.x << " y: " << _velocity.y << " z: " << _velocity.z << std::endl;
}
void Character::moveMouse(const MouseMoveEvent* event)
{
    glm::vec2 currentCurserPos = event->getPos();
    glm::vec2 deltaPos = currentCurserPos - _lastCursorPos;
    _lastCursorPos = currentCurserPos;
    rotateY(deltaPos.y * RotationSpeed);
    rotateX(deltaPos.x * RotationSpeed);
    //Debug
    // std::cout << rotation.x << std::endl;
    // std::cout << rotation.y << std::endl;
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
    // _eventManager->handleEvent(new CollectLootEvent(11.5f));
}
Character::Character()
{
    // _manager = manager;

    // EventManager *myEventManager = EventManager::Instance();
    // myEventManager->subscribe<Character, int, int>("key", this, &Character::moveCharacter);
    // myEventManager->subscribe<Character, double, double>("mouse", this, &Character::moveMouse);

    // _manager->subscribeToKey(GLFW_KEY_W, [=](int keyID, int action){
    //     std::cout << keyID << std::endl;
    // });
    // InputManager *m = InputManager::Instance();
    // m->createEvent("testEvent");
    // m->subscribe("testEvent", this, &Character::cb);
    // _manager->subscribeToKey(GLFW_KEY_A, callback, this);
    // _manager->subscribeToKey(GLFW_KEY_S, callback, this);
    // _manager->subscribeToKey(GLFW_KEY_D, callback, this);
    // _camera = Camera(float fov, float aspect, float near, float far);
}
Character::~Character()
{

}
