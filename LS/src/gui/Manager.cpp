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
        _manager->listen(this, &Manager::resizeWindow);
    }
    Manager::~Manager()
    {
        for (View* view : _loadedViews)
        {
            delete view;
        }
    }
    void Manager::update(float dt)
    {
        if (_currentView != nullptr)
        {
            _currentView->update(dt);
        }
    }
    void Manager::render(float dt)
    {
        if (_currentView != nullptr)
        {
            _currentView->render(dt);
        }
    }
    View *Manager::setView(std::string name)
    {
        for (View* view : _loadedViews)
        {
            if (view->getName() == name)
            {
                if (_currentView != nullptr)
                    _currentView->pauseView();
                _currentView = view;
                _currentView->initiate();
                _currentView->resumeView();
                _currentView->update(0.0f);
                return _currentView;
            }
        }
        return nullptr;
    }
    void Manager::setView(View* view)
    {
        if (_currentView != nullptr)
            _currentView->pauseView();
        _currentView = view;
        _currentView->setParent(this);
        _currentView->initiate();
        _currentView->update(0.0f);
        //add if not existing
        for (View* v : _loadedViews)
        {
            if (v->getName() == view->getName())
            {
                return;
            }
        }
        _loadedViews.push_back(view);
    }
    View *Manager::resumeView(std::string name)
    {
        for (View* view : _loadedViews)
        {
            if (view->getName() == name)
            {
                if (_currentView != nullptr)
                    _currentView->pauseView();
                _currentView = view;
                _currentView->resumeView();
                return _currentView;
            }
        }
        return nullptr;
    }
    void Manager::setWindowSize(unsigned int width, unsigned int height)
    {
        _windowWidth = width;
        _windowHeight = height;
    }
    void Manager::cursorMoved(const MouseMoveEvent& event)
    {
        if (_cursorMode == GLFW_CURSOR_NORMAL && _currentView != nullptr)
        {
            glm::vec2 pos = event.getPos();
            _lastCursorPos.x = (pos.x / (float)_windowWidth * 2) - 1;
            _lastCursorPos.y = (1 - (pos.y / (float)_windowHeight * 2));
            if (_lastCursorPos.x >= -1 && _lastCursorPos.y >= -1 && _lastCursorPos.x <= 1 && _lastCursorPos.y <= 1)
            {
                Element *e = _currentView->View::checkCollision(_lastCursorPos);
                if (e != nullptr)
                {
                    if (_selectedElement == nullptr)
                    {
                        _selectedElement = e;
                        e->cursorDidEnter(_lastCursorPos);
                    }
                    else if (_selectedElement != e)
                    {
                        _selectedElement->cursorDidExit(_lastCursorPos);
                        _selectedElement = e;
                        e->cursorDidEnter(_lastCursorPos);
                    }
                    else
                    {
                        _selectedElement->cursorMovedInside(_lastCursorPos);
                    }
                }
                else
                {
                    if (_selectedElement != nullptr)
                    {
                        _selectedElement->cursorDidExit(_lastCursorPos);
                        _selectedElement = nullptr;
                    }
                }
            }
        }
    }

    unsigned int Manager::getWindowWidth()
    {
        return _windowWidth;
    }
    unsigned int Manager::getWindowHeight()
    {
        return _windowHeight;
    }
    void Manager::mouseClick(const MouseClickEvent& event)
    {
        if (_cursorMode == GLFW_CURSOR_NORMAL)
        {
            if (_currentView != nullptr)
            {
                if (_selectedElement != nullptr)
                {
                    _selectedElement->handleClick(event.getAction(), _lastCursorPos);
                }
            }
        }
    }
    void Manager::cursorModeChanged(const cursorModeChangeEvent& event)
    {
        _cursorMode = event.getState();
    }
    void Manager::resizeWindow(const ResizeWindowEvent& event)
    {
        _windowWidth = event._width;
        _windowHeight = event._height;
    }
}
