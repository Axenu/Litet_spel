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

    }
    ProgressBar::~ProgressBar()
    {

    }

    void ProgressBar::onRender()
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
