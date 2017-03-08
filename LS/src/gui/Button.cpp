#include "gui/Button.h"

namespace gui
{
    Button::Button() : Element()
    {
        _font = Factory::getInstance().getFont("Resources/fonts/arial");
        _label = new Label(_font);
        _label->setZ(51);
        _padding = glm::vec2(0.05f);
        _rect = new Rectangle(_label->getTextWidth() + _padding.x * 2, _label->getTextHeight() + _padding.y * 2);
        _primaryColor = glm::vec4(0.5,0,0,1);
        _secondaryColor = glm::vec4(0.5,0.5,0,1);
        _rect->setColor(_primaryColor);
        _label->setPosition(_padding.x, _padding.y);
        _label->updateText();
        _size = _rect->getSize();
        _isReactive = true;
        addChild(_rect);
        addChild(_label);
        _cursorInside = false;
    }
    Button::Button(float width, float height) : Button()
    {
        _size.x = width;
        _size.y = height;
        _rect->setScale(width, height);
    }
    Button::~Button()
    {
		if (_callback)
			delete _callback;
    }
    void Button::onRender(float dt)
    {

    }
    void Button::onUpdate(float dt)
    {
        _label->setPosition((_size.x - _label->getTextWidth())*0.5f,(_size.y-_label->getTextHeight())*0.5f);
    }
    // setters
    void Button::setPrimaryColor(glm::vec4 color)
    {
        _primaryColor = color;
        if (!_selected)
            if (!_cursorInside)
                _rect->setColor(color);
    }
    void Button::setSecondaryColor(glm::vec4 color)
    {
        _secondaryColor = color;
        if (_cursorInside || _selected)
            _rect->setColor(color);
    }
    void Button::setTextColor(glm::vec4 color)
    {
        _textColor = color;
        _label->setColor(color);
    }
    void Button::setPadding(float x, float y)
    {
        _padding.x = x;
        _padding.y = y;
    }
    void Button::execute(int action)
    {

    }
    void Button::cursorDidEnter(glm::vec2 pos)
    {
        _rect->setColor(_secondaryColor);
        _cursorInside = true;
    }
    void Button::cursorDidExit(glm::vec2 pos)
    {
        if (!_selected)
            _rect->setColor(_primaryColor);
        _cursorInside = false;
    }
    bool Button::handleClick(int action, glm::vec2 pos)
    {
        if (_callback != nullptr)
        {
            _callback->exec(action);
            return true;
        }
        return false;
    }
    void Button::addStringComponent(StringComponent* sc)
    {
        _label->addStringComponent(sc);
        onUpdate(0.0f);
    }
    void Button::setSelected(bool select)
    {
        _selected = select;
        if (_selected)
            _rect->setColor(_secondaryColor);
        else
            _rect->setColor(_primaryColor);
    }
}
