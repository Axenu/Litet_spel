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
        EventManager* _eventManager;
        //owned and deleted by subclass View -> Element
        Label *_tipDisplay;
        Label *_scoreLabel;
        Label *_grenadeCountLabel;
		Label *_grenadeCooldownCounter;
        Rectangle *_scoreBackground;
        Label *_soundLabel;
        Label *_lightLabel;
        Label *_grenadeLabel;
        ProgressBar *_lightPB;
        ProgressBar *_soundPB;
        ProgressBar *_guardVisionPB;
        Label *_guardVisionLabel;
        //owned by this
        std::unique_ptr<TestGame> _game;
        //pointer to float not owned by this
        float* _fps;

        bool _isAtExit = false;
        std::vector<AlertStruct> _alerts;

    public:
        HUDView(EventManager* manager, float* fps);
        virtual ~HUDView();

        void onRender();
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
