#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <IL/il.h>
#include "gui/Element.h"
#include "Shader.h"

namespace gui
{

class TexturedRectangle : public Element
{
private:
    glm::vec4 _color;

    //GL stuff
    GLint _positionUniform;
    GLint _colorUniform;;
    GLint _sizeUniform;
    GLint _textureUniform;
    GLuint _texture;
    gl::VAData _VA;

    Shader* _shader;

    void createTexture(std::string path);

public:

    TexturedRectangle(float width, float height, std::string path);
    ~TexturedRectangle();

    void render();

    void update(float dt);

};

}
