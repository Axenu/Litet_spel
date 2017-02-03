#include "gui/Button.h"

namespace gui
{
    Button::Button(std::string text) : Element()
    {
        Font *f = new Font("Resources/fonts/arial");
        _label = new Label(f, text);
        _label->setZ(51);
        // get label size
        // set rect to labelsize + padding
        // move label to middle of rect

        float padding = 0.05f;

        _rect = new Rectangle(_label->getTextWidth() + padding * 2, _label->getTextHeight() + padding * 2);
        glm::vec4 color(0.5,0,0,1);
        _rect->setColor(color);
        _label->setPosition(padding, padding);
        _size = _rect->getSize();
        addChild(_rect);
        addChild(_label);
    }
    Button::~Button()
    {

    }
    void Button::onRender()
    {

    }
    void Button::onUpdate(float dt)
    {

    }
    bool Button::handleClick(int action)
    {
        std::cout << "Clicked button with action: " << action << std::endl;
        return true;
    }
}
