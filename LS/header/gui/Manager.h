#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Shader.h"
#include "Font.h"
#include "gui/Element.h"
#include "gui/GUIScene.h"
#include "InputManager.h"

//new
#include "EventManager.h"

namespace gui
{

    class Manager
    {
    private:
        EventManager* _manager;
        Scene* _currentScene;
        glm::vec2 _lastCursorPos;
        glm::vec2 _windowSize;
        int _cursorMode = GLFW_CURSOR_NORMAL;
        std::vector<Scene*> _loadedScenes;

    public:
        Manager();
        Manager(EventManager* manager);
        ~Manager();

        bool setScene(std::string name);
        void setScene(Scene* scene);
        void setWindowSize(float width, float height);

        void update(float dt);
        void render();

        void cursorMoved(const MouseMoveEvent& event);
        void mouseClick(const MouseClickEvent& event);
        void cursorModeChanged(const cursorModeChangeEvent& event);

    };

}
