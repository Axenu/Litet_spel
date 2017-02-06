#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Shader.h"
#include "Font.h"
#include "gui/Element.h"
#include "gui/StringComponents.h"

namespace gui
{

    class Label : public Element
    {
    private:
        glm::vec4 _color;
        // glm::vec2 _textSize;

        std::string _text;
        std::vector<StringComponent*> _sComponents;

        //GL stuff
        GLint _positionZUniform;
        GLint _colorUniform;
        GLint _modelMatrixUniform;
        gl::VAData _VA;

        Shader _shader;
        Font* _font;

    public:

        Label(Font* font, std::string text);
    	virtual ~Label();

        void onRender();

        void onUpdate(float dt);

        void updateText();

        //setters
        void addStringComponent(StringComponent* sc);

        //getters
        float getTextWidth();
        float getTextHeight();
        glm::vec2& getTextSize();

    };

}
