#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Shader.h"
#include "Font.h"
#include "gui/Element.h"

namespace gui
{

class Label : public Element
{
private:
    glm::vec4 _color;

    std::string _text;

    //GL stuff
    GLint _positionUniform;
    GLint _textureUniform;
    GLint _colorUniform;
    GLint _sizeUniform;
    gl::VAData _VA;

    Shader* _shader;
    Font* _font;

public:

    Label(Font* font, std::string text);
    ~Label();

    void render();

    void update(float dt);

    void setText(std::string &text);

};

}
