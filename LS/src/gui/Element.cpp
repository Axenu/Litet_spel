#include "gui/Element.h"

namespace gui
{
    Element::Element()
    {
        _size = glm::vec2(1,1);
        _scale = glm::vec2(1,1);
        _position.z = -0.001f;
    }
    Element::~Element()
    {
        for (Element* child : _children)
        {
            delete child;
        }
    }
    void Element::render()
    {
        onRender();
        for (Element* child : _children)
        {
            child->render();
        }
    }
    void Element::update(float dt)
    {
        _modelMatrix = glm::mat3();
        _modelMatrix = glm::translate(_modelMatrix, glm::vec2(_position.x, _position.y));
        _modelMatrix = glm::scale(_modelMatrix, glm::vec2(_scale.x,_scale.y));
        if (_parent != nullptr) {
            _modelMatrix = _parent->_modelMatrix * _modelMatrix;
        }
        for (Element *child : _children)
        {
            child->update(dt);
        }
        onUpdate(dt);
    }
    void Element::addChild(Element* child)
    {
        child->_parent = this;
        _children.push_back(child);
    }
    void Element::testClick(glm::vec2& pos, int action)
    {
        //test collision with this
        glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
        glm::vec3 max = _modelMatrix * glm::vec3(_size.x, _size.y, 1.0);
        if (pos.x < max.x && pos.x > min.x)
        {
            if (pos.y < max.y && pos.y > min.y)
            {
                if (!handleClick(action))
                {
                    for (Element* child : _children)
                    {
                        child->testClick(pos, action);
                    }
                }
            }
        }
    }
    bool Element::handleClick(int action)
    {
        return false;
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
    glm::vec2& Element::getScale()
    {
        return _scale;
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
    void Element::setZ(float z)
    {
        z = std::max(z, 0.0f);
        z = std::min(z, 99.0f);
        _position.z = -z/100.0f - 0.001f;
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
    void Element::setScale(glm::vec2 &scale)
    {
        _scale = scale;
    }
    void Element::setScale(float x, float y)
    {
        _scale.x = x;
        _scale.y = y;
    }
    void Element::setScale(float s)
    {
        _scale.x = s;
        _scale.y = s;
    }
}
