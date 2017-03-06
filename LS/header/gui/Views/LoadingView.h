#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gui/Font.h"
#include "gui/View.h"
#include "Event/Events.h"
#include "gui/Views/HUDView.h"
#include "staticVars.h"

namespace gui
{

    class LoadingView : public View
    {
    private:
        //passed from main
        EventManager* _manager;
        //pointer to float not owned by this
        float* _fps;
        //local only
        float _firstUpdate = true;

    public:
        LoadingView(EventManager* manager, float* fps);
        virtual ~LoadingView();

        void onRender();
        void onUpdate(float dt);
        void pauseView() {}
        void resumeView() {}

        virtual void initiate();

    };

}
