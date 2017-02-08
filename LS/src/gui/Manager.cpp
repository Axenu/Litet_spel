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
    void Manager::render()
    {
        if (_currentView != nullptr)
        {
            _currentView->render();
        }
    }
    bool Manager::setView(std::string name)
    {
        for (View* view : _loadedViews)
        {
            if (view->getName() == name)
            {
                _currentView = view;
                _currentView->initiate();
                return true;
            }
        }
        return false;
    }
    void Manager::setView(View* view)
    {
        _currentView = view;
        _currentView->setParent(this);
        _currentView->initiate();
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
                // std::cout << "x: " << pos.x << " y: " << pos.y << std::endl;
                // std::cout << "ds" << std::endl;
                Element *e = _currentView->View::checkCollision(pos);
                if (e != nullptr)
                {
                    // std::cout << "col" << std::endl;
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
