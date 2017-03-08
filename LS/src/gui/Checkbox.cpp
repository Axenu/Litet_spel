#include "gui/Checkbox.h"


namespace gui {
    Checkbox::Checkbox(float width, float height)
    {
        _size.x = width;
        _size.y = height;

        Font *font = Factory::getInstance().getFont("Resources/fonts/arial");

        _backgroundRect = new Rectangle(_size.x, _size.y);
        addChild(_backgroundRect);
        _foregroundRect = new Rectangle(_size.x * 0.8f, _size.y * 0.8f);
        _foregroundRect->setPosition(_size.x * 0.1f, _size.y * 0.1f);
        _foregroundRect->setZ(2);
        addChild(_foregroundRect);
        _foregroundRect->deactivate();
        _isReactive = true;
        _selected = false;

        // Label *l;
        // for (int i = 0; i < count; i++)
        // {
        //     l = new Label(font);
        //     l->addStringComponent(new StringComponentString(choises[i]));
        //     l->setPosition(0.35f * i + 0.175f - l->getTextWidth() * 0.125f, 0.06f - l->getTextHeight()*0.125f);
        //     l->setScale(0.25f);
        //     l->setZ(3);
        //     addChild(l);
        // }
    }
    Checkbox::~Checkbox()
    {

    }
    void Checkbox::onRender(float dt)
    {

    }
    void Checkbox::onUpdate(float dt)
    {

    }
    void Checkbox::cursorDidEnter(glm::vec2 pos)
    {

    }
    void Checkbox::cursorDidExit(glm::vec2 pos)
    {

    }
    void Checkbox::cursorMovedInside(glm::vec2 pos)
    {

    }
    bool Checkbox::handleClick(int action, glm::vec2 pos)
    {
        if (action == GLFW_PRESS)
        {
            if (_selected)
            {
                _selected = false;
                _foregroundRect->deactivate();
            }
            else
            {
                _selected = true;
                _foregroundRect->activate();
            }
        }
        return false;
    }

    // setters
    void Checkbox::setPrimaryColor(glm::vec4 color)
    {
        _primaryColor = color;
        _backgroundRect->setColor(color);
    }
    void Checkbox::setSecondaryColor(glm::vec4 color)
    {
        _secondaryColor = color;
        _foregroundRect->setColor(color);
    }
    void Checkbox::setSelected(bool selected)
    {
        if (!selected)
        {
            _selected = false;
            _foregroundRect->deactivate();
        }
        else
        {
            _selected = true;
            _foregroundRect->activate();
        }
    }
    bool Checkbox::getSelected()
    {
        return _selected;
    }
}
