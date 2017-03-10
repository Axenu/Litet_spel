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
#include "gui/ProgressBar.h"
#include "gui/ChoiseBar.h"
#include "gui/Checkbox.h"
#include "Event/Input/InputManager.h"
#include "Event/Events.h"
#include "Config.h"
#include "Sound/SoundManager.h"

namespace gui
{

    class InGameSettingsView : public View
    {
    private:
        //owned and deleted by subclass View -> Element
        Button* _backButton;
        ProgressBar* _soundPB;
        Checkbox *_soundCheckbox;

    public:
        InGameSettingsView();
        virtual ~InGameSettingsView();

        void onRender(float dt) {}
        void onUpdate(float dt) {}
        void pauseView() {}
        void resumeView() {}

        virtual void initiate() {}

        void back(int action);
    };

}
