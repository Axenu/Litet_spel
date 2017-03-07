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
            _lastCursorPos = event.getPos();
            glm::vec2 pos;
            pos.x = (_lastCursorPos.x / (float)_windowWidth * 2) - 1;
            pos.y = (1 - (_lastCursorPos.y / (float)_windowHeight * 2));
            if (pos.x >= -1 && pos.y >= -1 && pos.x <= 1 && pos.y <= 1)
            {
                Element *e = _currentView->View::checkCollision(pos);
                if (e != nullptr)
                {
                    if (_selectedElement == nullptr)
                    {
                        _selectedElement = e;
                        e->cursorDidEnter();
                    }
                    else if (_selectedElement != e)
                    {
                        _selectedElement->cursorDidExit();
                        _selectedElement = e;
                        e->cursorDidEnter();
                    }
                }
                else
                {
                    if (_selectedElement != nullptr)
                    {
                        _selectedElement->cursorDidExit();
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
                    _selectedElement->handleClick(event.getAction());
                }
            }
        }
    }
    void Manager::cursorModeChanged(const cursorModeChangeEvent& event)
    {
        _cursorMode = event.getState();
    }
}
