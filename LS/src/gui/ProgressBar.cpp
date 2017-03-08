#include "gui/ProgressBar.h"

namespace gui
{
    ProgressBar::ProgressBar() : ProgressBar(0.5f, 0.1f)
    {

    }
    ProgressBar::ProgressBar(float width, float height)
    {
        _size.x = width;
        _size.y = height;
        _backgroundRect = new Rectangle(width, height);
        addChild(_backgroundRect);

        _foregroundRect = new Rectangle(width * 0.5f, height);
        _foregroundRect->setZ(2);
        addChild(_foregroundRect);
        _value = 0.5f;
        _inverted = false;
        _clicked = false;

    }
    ProgressBar::~ProgressBar()
    {

    }

    void ProgressBar::onRender(float dt)
    {

    }
    void ProgressBar::onUpdate(float dt)
    {
        if (_inverted)
        {
            _foregroundRect->setPosition(_size.x * (1 - _value), 0);
            _foregroundRect->setScale(_size.x * _value, _size.y);
        }
        else
        {
            _foregroundRect->setScale(_size.x * _value, _size.y);
        }
    }

    //cursor handling
    void ProgressBar::cursorDidEnter(glm::vec2 pos)
    {
        // _rect->setColor(_secondaryColor);
        // _cursorInside = true;
    }
    void ProgressBar::cursorDidExit(glm::vec2 pos)
    {
        // _rect->setColor(_primaryColor);
        // _cursorInside = false;
    }
    void ProgressBar::cursorMovedInside(glm::vec2 pos)
    {
        if (_clicked)
        {
            glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
            glm::vec3 max = _modelMatrix * glm::vec3(_size.x, _size.y, 1.0);
            _value = (pos.x - min.x)/(max.x - min.x);
            if (_inverted)
            {
                _value = 1 - _value;
            }
        }
    }
    bool ProgressBar::handleClick(int action, glm::vec2 pos)
    {
        if (action == GLFW_PRESS)
        {
            glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
            glm::vec3 max = _modelMatrix * glm::vec3(_size.x, _size.y, 1.0);
            _value = (pos.x - min.x)/(max.x - min.x);
            if (_inverted)
            {
                _value = 1 - _value;
            }
            _clicked = true;
        }
        else if (action == GLFW_RELEASE)
        {
            _clicked = false;
        }
        return false;
    }

    // setters
    void ProgressBar::setPrimaryColor(glm::vec4 color)
    {
        _primaryColor = color;
        _backgroundRect->setColor(color);
    }
    void ProgressBar::setSecondaryColor(glm::vec4 color)
    {
        _secondaryColor = color;
        _foregroundRect->setColor(color);
    }
    void ProgressBar::setValue(float value)
    {
        _value = value;
    }
    void ProgressBar::setInverted(bool i)
    {
        _inverted = i;
    }
    //getters
    float ProgressBar::getValue()
    {
        return _value;
    }
}
