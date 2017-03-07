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
        //passed from main
        EventManager* _manager;
        //owned and deleted by subclass View -> Element
        Button *_playAgainButton;
        Button* _quitButton;
        Button* _mainMenuButton;
        Label *_victoryLabel;
        Label *_scoreLabel;
        Label *_lootLabel;

        float _score;
        int _loot;

    public:
        GameOverView(EventManager* manager, const GameOverEvent &event);
        virtual ~GameOverView();

        void onRender(float dt);
        void onUpdate(float dt);
        void pauseView() {}
        void resumeView() {}

        virtual void initiate();
        void updateText(const GameOverEvent &event);

        void setScoreAndLoot(float score, int loot);

        void QuitGame(int action);
        void gotoMainMenu(int action);
        void playAgain(int action);

        // void gameStarted(const GameStartedEvent &event);
        // void gameOver(const GameOverEvent &event);

        // void QuitGame(int action);
        // void StartGame(int action);
    };

}
