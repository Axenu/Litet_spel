#include "gui/Element.h"

namespace gui
{
    Element::Element()
    {
        _size = glm::vec2(1,1);
        _position.z = -0.001;
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
        for (Element* child : _children)
        {
            child->render();
        }
        onRender();
    }
    void Element::update(float dt)
    {
        _positionGlobal = _position;
        _modelMatrix = glm::mat3();
        _modelMatrix = glm::translate(_modelMatrix, glm::vec2(_position.x, _position.y));
        _modelMatrix = glm::scale(_modelMatrix, glm::vec2(_size.x,_size.y));
    	// _modelMatrix[2] = glm::vec3(-0.3, _position.y, 1.0);
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
        if (pos.x < _positionGlobal.x + _size.x && pos.x > _positionGlobal.x)
        {

            if (pos.y < _positionGlobal.y + _size.y && pos.y > _positionGlobal.y)
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
        z = fmax(z, 0.0f);
        z = fmin(z, 99.0f);
        _position.z = -z/100.0f - 0.001;
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
