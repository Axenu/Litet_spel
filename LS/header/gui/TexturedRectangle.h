#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <IL/il.h>
#include "gl/GLFunctions.h"
#include "gui/Element.h"
#include "Render/Shader.h"
#include "Factory.h"

namespace gui
{

    class TexturedRectangle : public Element
    {
    private:
        glm::vec4 _color;

        //GL stuff
        GLint _positionZUniform;
        GLint _colorUniform;
        GLint _modelMatrixUniform;
        GLint _textureUniform;
        GLuint _texture;
        gl::VAData _VA;

        Shader* _shader;

        void createTexture(std::string path);

    public:

        TexturedRectangle(float width, float height, std::string path);
       virtual ~TexturedRectangle();

        void onRender();

        void onUpdate(float dt);

        void setColor(glm::vec4& color);
        void setOpacity(float opacity);

    };

}
