#include "gui/Button.h"

namespace gui
{
    Button::Button() : Element()
    {
        _font = new Font("Resources/fonts/arial");
        _label = new Label(_font);
        _label->setZ(51);
        _padding = 0.05f;
        _rect = new Rectangle(_label->getTextWidth() + _padding * 2, _label->getTextHeight() + _padding * 2);
        glm::vec4 color(0.5,0,0,1);
        _rect->setColor(color);
        _label->setPosition(_padding, _padding);
        _label->updateText();
         _size = _rect->getSize();
        addChild(_rect);
        addChild(_label);
    }
    Button::~Button()
    {
		if (_callback)
			delete _callback;
		if (_font)
			delete _font;
    }
    void Button::onRender()
    {

    }
    void Button::onUpdate(float dt)
    {
        _rect->setScale(_label->getTextWidth() + _padding * 2, _label->getTextHeight() + _padding * 2);
        setSize(_label->getTextWidth() + _padding * 2, _label->getTextHeight() + _padding * 2);
    }
    void execute(int action)
    {

    }
    bool Button::handleClick(int action)
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
        _rect->setScale(_label->getTextWidth() + _padding * 2, _label->getTextHeight() + _padding * 2);
        setSize(_label->getTextWidth() + _padding * 2, _label->getTextHeight() + _padding * 2);
    }
}
