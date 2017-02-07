#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Shader.h"
#include "Font.h"
#include "gui/GUIScene.h"
#include "gui/Button.h"
#include "Event/Input/InputManager.h"
#include "Event/Events.h"

namespace gui
{

    class GameOverScene : public Scene
    {
    private:
        EventManager* _manager;
        gui::Font *_font;
        // gui::Button* _quitButton;
        // gui::Button* _startButton;
        // float* _fps;

    public:
        GameOverScene(EventManager* manager, const GameOverEvent &event);
        virtual ~GameOverScene();

        void onRender();
        void onUpdate(float dt);

        // void gameStarted(const GameStartedEvent &event);
        // void gameOver(const GameOverEvent &event);

        // void QuitGame(int action);
        // void StartGame(int action);
    };

}
