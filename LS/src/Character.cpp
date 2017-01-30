#include "Character.h"

void Character::setCamera(Camera* camera)
{
    _camera = camera;
}
void Character::onUpdate(float dt)
{
    // std::cout << dt << std::endl;
    _camera->setRX(rotation.x);
    _camera->setRY(rotation.x);
    _camera->moveX(_velocity.y * dt * sin(rotation.y));
    _camera->moveZ(_velocity.y * dt * cos(rotation.y));
    _camera->moveX(_velocity.x * dt * cos(rotation.y));
    _camera->moveZ(_velocity.x * dt * sin(rotation.y));

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
            _velocity.y += 1.f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.y -= 1.f;
        }
    }
    else if (event->getKey() == GLFW_KEY_A)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x -= 1.f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.x += 1.f;
        }
    }
    else if (event->getKey() == GLFW_KEY_S)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.y -= 1.f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.y += 1.f;
        }
    }
    else if (event->getKey() == GLFW_KEY_D)
    {
        if (event->getAction() == GLFW_PRESS)
        {
            _velocity.x += 1.f;
        } else if (event->getAction() == GLFW_RELEASE) {
            _velocity.x -= 1.f;
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
    rotateY(deltaPos.x * RotationSpeed);
    rotateX(deltaPos.y * RotationSpeed);
    //Debug
    // std::cout << event->getX() << std::endl;
    // std::cout << event->getY() << std::endl;
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
    _eventManager->registerEventFunc(this, &Character::moveCharacter);
    _eventManager->registerEventFunc(this, &Character::moveMouse);
    _eventManager->registerEventFunc(this, &Character::collectLoot);
    _eventManager->handleEvent(new CollectLootEvent(11.5f));
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
