#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include  "gl/GLFunctions.h"
#include "Element.h"
#include "Rectangle.h"
#include "Label.h"

namespace gui
{

    class Button : public Element
    {
    private:
        Label *_label;
        Rectangle *_rect;

    public:

        Button(std::string text);
        ~Button();

        void onRender();

        void onUpdate(float dt);

        bool handleClick(int action);

    };

}
