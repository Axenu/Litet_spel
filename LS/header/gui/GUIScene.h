#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Shader.h"
#include "Font.h"
#include "gui/Element.h"

namespace gui
{

    class Scene : public Element
    {
    private:
        int _temp;

    public:
        Scene();
        ~Scene();

        void onRender();
        void onUpdate(float dt);
    };

}
