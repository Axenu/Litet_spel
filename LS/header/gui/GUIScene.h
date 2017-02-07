#pragma once

#include <iostream>
#include <vector>
#include"gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "gl/GLFunctions.h"
#include "Font.h"
#include "gui/Element.h"

namespace gui
{

    class Manager;

    class Scene : public Element
    {
    private:
    protected:
        std::string _name;
        Manager* _parent;
    public:
        Scene();
		virtual ~Scene();

        void onRender();
        void onUpdate(float dt);

        std::string getName();

        void setParent(gui::Manager *manager);
        void testClick(glm::vec2& pos, int action);
    };

}
