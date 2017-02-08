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

    class View : public Element
    {
    private:
    protected:
        std::string _name;
        Manager* _parent;
    public:
        View();
		virtual ~View();

        void onRender();
        void onUpdate(float dt);

        virtual void initiate() = 0;

        std::string getName();

        void setParent(gui::Manager *manager);
        void testClick(glm::vec2& pos, int action);
    };

}
