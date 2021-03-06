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
#include "staticVars.h"

namespace gui
{

    class PauseView : public View
    {
    private:
        //passed from main
        EventManager* _eventManager;
        //owned and deleted by subclass View -> Element
        gui::Button* _resumeButton;
        gui::Button* _mainMenuButton;
        gui::Button* _quitButton;
        gui::Button* _settingsButton;

    public:
        PauseView(EventManager* manager);
        virtual ~PauseView();

        void onRender(float dt);
        void onUpdate(float dt);
        void pauseView() {}
        void resumeView() {}

        virtual void initiate();

        void quitGame(int action);
        void mainMenu(int action);
        void settingsMenu(int action);
        void resumeGame(int action);
    };

}
