#include "gui/View.h"

namespace gui
{
    View::View() : Element()
    {
        _size = glm::vec2(2,2);
        // _position.x = -1;
        // _position.y = -1;
        // std::cout << "gui scene constructor" << std::endl;
    }
    View::~View()
    {
        // std::cout << "gui scene destructor" << std::endl;
    }
    void View::onRender()
    {

    }
    void View::onUpdate(float dt)
    {

    }
    std::string View::getName()
    {
        return _name;
    }
    void View::setParent(gui::Manager *manager)
    {
        _parent = manager;
    }
    void View::testClick(glm::vec2& pos, int action)
    {
        for (Element* child : _children)
        {
            child->testClick(pos, action);
        }
    }
}
