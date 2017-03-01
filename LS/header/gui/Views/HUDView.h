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
#include "gui/TexturedRectangle.h"
#include "Event/Input/InputManager.h"
#include "Event/Events.h"
#include "gui/Views/GameOverView.h"
#include "Game/TestGame.h"
#include "Game/Objects/CharacterEvents.h"
#include "gui/ProgressBar.h"
#include "staticVars.h"

namespace gui
{

    class HUDView : public View
    {
    private:
        EventManager* _eventManager;
        gui::Font *_font;
        Label *_tipDisplay;
        Label *_scoreLabel;
        Label *_grenadeCountLabel;
		Label *_grenadeCooldownCounter;
        ProgressBar *_lightPB;
        ProgressBar *_soundPB;
        TestGame *_game = nullptr;
        float* _fps;
        bool _isAtExit = false;

    public:
        HUDView(EventManager* manager, float* fps);
        virtual ~HUDView();

        void onRender();
        void onUpdate(float dt);

        virtual void initiate();

        void gameStarted(const GameStartedEvent &event);
        void gameOver(const GameOverEvent &event);
        void exitSquareTrigger(const CharacterSquareEvent &event);
		void canClimb(const CanClimbEvent &event);

        // void endGame(int action);

        // void QuitGame(int action);
        // void StartGame(int action);
    };

}
