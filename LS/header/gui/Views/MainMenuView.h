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
#include "gui/Views/LoadingView.h"
#include "gui/Views/CreditsView.h"
#include "staticVars.h"
#include "gui/ProgressBar.h"

namespace gui
{

    class MainMenuView : public View
    {
    private:
        //passed from main
        EventManager* _manager;
        //owned and deleted by subclass View -> Element
        gui::Button* _startButton;
        gui::Button* _settingsButton;
        gui::Button* _creditsButton;
        gui::Button* _quitButton;
        //pointer to float not owned by this
        float* _fps;

    public:
        MainMenuView(EventManager* manager, float* fps);
        virtual ~MainMenuView();

        void onRender(float dt);
        void onUpdate(float dt);
        void pauseView() {}
        void resumeView() {}

        virtual void initiate();

        void QuitGame(int action);
        void StartGame(int action);
        void Credits(int action);
        void Settings(int action);
    };

}
