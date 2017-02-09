#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gui/Font.h"
#include "gui/View.h"
#include "gui/Button.h"
#include "Event/Input/InputManager.h"
#include "Event/Events.h"

namespace gui
{

    class GameOverView : public View
    {
    private:
        EventManager* _manager;
        gui::Font *_font;
        gui::Button* _quitButton;
        gui::Button* _mainMenuButton;
        int _score;
        // float* _fps;

    public:
        GameOverView(EventManager* manager, const GameOverEvent &event);
        virtual ~GameOverView();

        void onRender();
        void onUpdate(float dt);

        virtual void initiate();

        void setScore(int score);

        void QuitGame(int action);
        void gotoMainMenu(int action);

        // void gameStarted(const GameStartedEvent &event);
        // void gameOver(const GameOverEvent &event);

        // void QuitGame(int action);
        // void StartGame(int action);
    };

}
