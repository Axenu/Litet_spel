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
#include "gui/Views/HUDView.h"
#include "staticVars.h"
#include "gui/ProgressBar.h"

namespace gui
{

    class MainMenuView : public View
    {
    private:
        EventManager* _manager;
        gui::Font *_font = nullptr;
        gui::Button* _startButton;
        gui::Button* _settingsButton;
        gui::Button* _creditsButton;
        gui::Button* _quitButton;
        float* _fps;

    public:
        MainMenuView(EventManager* manager, float* fps);
        virtual ~MainMenuView();

        void onRender();
        void onUpdate(float dt);

        virtual void initiate();

        void QuitGame(int action);
        void StartGame(int action);
    };

}
