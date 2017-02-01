#include "gui/Element.h"

namespace gui
{
    Element::Element()
    {
        _size = glm::vec2(1,1);
        _position.z = -0.5;
    }
    Element::~Element()
    {

    }
    //getters
    glm::vec3& Element::getPosition()
    {
        return _position;
    }
    glm::vec2& Element::getSize()
    {
        return _size;
    }
    //setters
    void Element::setPosition(glm::vec2 &pos)
    {
        _position.x = pos.x;
        _position.y = pos.y;
    }
    void Element::setPosition(float x, float y)
    {
        _position.x = x;
        _position.y = y;
    }
    void Element::setSize(glm::vec2 &size)
    {
        _size = size;
    }
    void Element::setSize(float width, float height)
    {
        _size.x = width;
        _size.y = height;
    }
}
