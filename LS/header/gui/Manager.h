#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Font.h"
#include "gui/Element.h"
#include "gui/View.h"
#include "Event/Input/InputManager.h"

//new
#include "Event/EventManager.h"

namespace gui
{

    class Manager
    {
    private:
        EventManager* _manager;
        View* _currentView = nullptr;
        glm::vec2 _lastCursorPos;
        int _cursorMode = GLFW_CURSOR_NORMAL;
        std::vector<View*> _loadedViews;
        unsigned int _windowWidth;
        unsigned int _windowHeight;
        Element *_selectedElement = nullptr;

    public:
        Manager();
        Manager(EventManager* manager);
        ~Manager();

        View *setView(std::string name);
        void setView(View* view);
        View *resumeView(std::string name);
        void setWindowSize(unsigned int width, unsigned int height);

        unsigned int getWindowWidth();
        unsigned int getWindowHeight();

        void update(float dt);
        void render(float dt);

        void cursorMoved(const MouseMoveEvent& event);
        void mouseClick(const MouseClickEvent& event);
        void cursorModeChanged(const cursorModeChangeEvent& event);
        void resizeWindow(const ResizeWindowEvent& event);

    };

}
