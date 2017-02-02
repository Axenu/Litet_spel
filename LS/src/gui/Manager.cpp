#include "gui/Manager.h"

namespace gui
{
    Manager::Manager()
    {

    }
    Manager::Manager(EventManager *manager)
    {
        _manager = manager;
        _manager->listen(this, &Manager::cursorMoved);
        _manager->listen(this, &Manager::mouseClick);
        _manager->listen(this, &Manager::cursorModeChanged);
    }
    Manager::~Manager()
    {

    }
    void Manager::update(float dt)
    {
        if (_currentScene != nullptr)
        {
            _currentScene->update(dt);
        }
    }
    void Manager::render()
    {
        if (_currentScene != nullptr)
        {
            _currentScene->render();
        }
    }
    void Manager::setScene(std::string path)
    {
        //TODO
    }
    void Manager::setScene(Scene* scene)
    {
        _currentScene = scene;
    }
    void Manager::setWindowSize(float width, float height)
    {
        _windowSize.x = width;
        _windowSize.y = height;
    }
    void Manager::cursorMoved(const MouseMoveEvent& event)
    {
        _lastCursorPos = event.getPos();
    }
    void Manager::mouseClick(const MouseClickEvent& event)
    {
        if (_currentScene != nullptr)
        {
            glm::vec2 pos;
            pos.x = _lastCursorPos.x / _windowSize.x * 2;
            pos.y = 2 - (_lastCursorPos.y / _windowSize.y * 2);
            if (pos.x >= 0 && pos.y >= 0 && pos.x <= 2 && pos.y <= 2)
            {
                _currentScene->testClick(pos, event.getAction());
                //test coliision with scene
            }
        }
    }
    void Manager::cursorModeChanged(const cursorModeChangeEvent& event)
    {

    }
}
