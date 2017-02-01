#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include"gl/GLFunctions.h"
#include "Element.h"
#include "Shader.h"

namespace gui
{

    class Rectangle : public Element
    {
    private:
        glm::vec4 _color;

        //GL stuff
        //Uniforms
        GLint _positionUniform;
        GLint _colorUniform;
        GLint _sizeUniform;
        //Mesh data
        gl::VAData _VA;
        //Shader
        Shader* _shader;

    public:

        Rectangle(float width, float height);
        ~Rectangle();

        void render();

        void update(float dt);

        //getters
        glm::vec4& getColor();
        //setters
        void setColor(glm::vec4& color);

    };

}
