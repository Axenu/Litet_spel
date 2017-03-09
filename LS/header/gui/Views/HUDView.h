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
#include <time.h>
#include <stdlib.h>


namespace gui
{

    struct AlertStruct
    {
        TexturedRectangle *_rect;
        float _detection;
        unsigned int _id;
    };
    class HUDView : public View
    {
    private:
        //passed from main
        EventManager* _eventManager;
        //owned and deleted by subclass View -> Element
        Label *_tipDisplay;
        Label *_scoreLabel;
        Label *_lootLabel;
        Label *_grenadeCountLabel;
		Label *_grenadeCooldownCounter;
        Rectangle *_scoreBackground;
        Rectangle *_lootBackground;
        Label *_soundLabel;
        Label *_lightLabel;
        Label *_grenadeLabel;
        ProgressBar *_lightPB;
        ProgressBar *_soundPB;
        ProgressBar *_guardVisionPB;
        Label *_guardVisionLabel;
        //owned by this
        TestGame *_game = nullptr;
        //pointer to float not owned by this
        float* _fps;

        bool _isAtExit = false;
        float _gameOverCD;
        std::vector<AlertStruct> _alerts;
        irrklang::ISound *_backgroundSound = nullptr;

        glm::vec4 mixColors(glm::vec4 color1, glm::vec4 color2, float dt);

    public:
        HUDView(EventManager* manager, float* fps);
        virtual ~HUDView();

        void onRender(float dt);
        void onUpdate(float dt);
        void pauseView();
        void resumeView();

        virtual void initiate();

        void KeyboardPressed(const KeyboardEvent &event);
        void switchToGuardVision(const GuardVisionEvent &event);
        void gameOver(const GameOverEvent &event);
        void exitSquareTrigger(const CharacterSquareEvent &event);
		void canClimb(const CanClimbEvent &event);
        void guardAlert(const GuardAlertEvent &event);
    };

}
