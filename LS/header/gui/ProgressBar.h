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
#include "StringComponents.h"

namespace gui
{

    class ProgressBar : public Element
    {
    private:
        Rectangle* _backgroundRect;
        Rectangle* _foregroundRect;
        glm::vec4 _primaryColor;
        glm::vec4 _secondaryColor;

        float _value;
        bool _inverted;
        bool _clicked;

    public:

        ProgressBar();
        ProgressBar(float width, float height);
		virtual ~ProgressBar();

        void onRender(float dt);
        void onUpdate(float dt);

        virtual void cursorDidEnter(glm::vec2 pos);
        virtual void cursorDidExit(glm::vec2 pos);
        void cursorMovedInside(glm::vec2 pos);
        virtual bool handleClick(int action, glm::vec2 pos);

        void setPrimaryColor(glm::vec4 color);
        void setSecondaryColor(glm::vec4 color);
        void setOpacity(float opacity);
        void setValue(float value);
        void setInverted(bool i);

        float getValue();

    };

}
