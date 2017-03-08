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

    class CreditsView : public View
    {
    private:
        //owned and deleted by subclass View -> Element
        gui::Button* _backButton;

    public:
        CreditsView();
        virtual ~CreditsView();

        void onRender(float dt) {}
        void onUpdate(float dt) {}
        void pauseView() {}
        void resumeView() {}

        virtual void initiate() {}

        void back(int action);
    };

}
