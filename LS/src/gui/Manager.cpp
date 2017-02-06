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
        for (Scene* scene : _loadedScenes)
        {
            delete scene;
        }
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
    bool Manager::setScene(std::string name)
    {
        for (Scene* scene : _loadedScenes)
        {
            if (scene->getName() == name)
            {
                _currentScene = scene;
                return true;
            }
        }
        return false;
    }
    void Manager::setScene(Scene* scene)
    {
        _currentScene = scene;
        _currentScene->setParent(this);
        //add if not existing
        for (Scene* s : _loadedScenes)
        {
            if (s->getName() == scene->getName())
            {
                return;
            }
        }
        _loadedScenes.push_back(scene);
    }
    void Manager::setWindowSize(float width, float height)
    {
        _windowSize.x = width;
        _windowSize.y = height;
    }
    void Manager::cursorMoved(const MouseMoveEvent& event)
    {
        if (_cursorMode == GLFW_CURSOR_NORMAL)
        {
            _lastCursorPos = event.getPos();
        }
    }
    void Manager::mouseClick(const MouseClickEvent& event)
    {
        if (_cursorMode == GLFW_CURSOR_NORMAL)
        {
            if (_currentScene != nullptr)
            {
                glm::vec2 pos;
                pos.x = (_lastCursorPos.x / _windowSize.x * 2) - 1;
                pos.y = (1 - (_lastCursorPos.y / _windowSize.y * 2));
                if (pos.x >= -1&& pos.y >= -1 && pos.x <= 1 && pos.y <= 1)
                {
                    _currentScene->Scene::testClick(pos, event.getAction());
                    //test coliision with scene
                }
            }
        }
    }
    void Manager::cursorModeChanged(const cursorModeChangeEvent& event)
    {
        _cursorMode = event.getState();
    }
}
