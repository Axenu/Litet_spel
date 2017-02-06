#include "gui/Button.h"

namespace gui
{
    Button::Button(std::string text) : Element()
    {
        Font *_font = new Font("Resources/fonts/arial");
        _label = new Label(_font, text);
        _label->setZ(51);
        float padding = 0.05f;
        _rect = new Rectangle(_label->getTextWidth() + padding * 2, _label->getTextHeight() + padding * 2);
        glm::vec4 color(0.5,0,0,1);
        _rect->setColor(color);
        _label->setPosition(padding, padding);
        // _label->addStringComponent(new StringComponentString("StringComponent: "));
        _label->addStringComponent(new StringComponentString("_pos: "));
        _label->addStringComponent(new StringComponentVec3(&(_position)));
        _label->updateText();
        // _size = _rect->getSize();
        addChild(_rect);
        addChild(_label);
    }
    Button::~Button()
    {
        // delete _font;
    }
    void Button::onRender()
    {

    }
    void Button::onUpdate(float dt)
    {
        float padding = 0.05f;
        _rect->setScale(_label->getTextWidth() + padding * 2, _label->getTextHeight() + padding * 2);
        setSize(_label->getTextWidth() + padding * 2, _label->getTextHeight() + padding * 2);
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
}
