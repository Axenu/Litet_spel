#include "gui/ChoiseBar.h"


namespace gui {
    ChoiseBar::ChoiseBar(std::string choises[], int count)
    {
        _size.x = 0.35f*count;
        _size.y = 0.12f;

        Font *font = Factory::getInstance().getFont("Resources/fonts/arial");

        _backgroundRect = new Rectangle(0.35f*count, 0.12f);
        addChild(_backgroundRect);
        _foregroundRect = new Rectangle(0.35f, 0.12f);
        _foregroundRect->setZ(2);
        addChild(_foregroundRect);
        _highlightRect = new Rectangle(0.35f, 0.12f);
        _highlightRect->setZ(2);
        _highlightRect->deactivate();
        addChild(_highlightRect);
        _isReactive = true;
        _numberOfChoises = count;

        Label *l;
        for (int i = 0; i < count; i++)
        {
            l = new Label(font);
            l->addStringComponent(new StringComponentString(choises[i]));
            l->setPosition(0.35f * i + 0.175f - l->getTextWidth() * 0.125f, 0.06f - l->getTextHeight()*0.125f);
            l->setScale(0.25f);
            l->setZ(3);
            addChild(l);
        }
    }
    ChoiseBar::~ChoiseBar()
    {

    }
    void ChoiseBar::onRender(float dt)
    {

    }
    void ChoiseBar::onUpdate(float dt)
    {

    }
    void ChoiseBar::cursorDidEnter(glm::vec2 pos)
    {
        _highlightRect->activate();
        glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
        float value = pos.x - min.x;
        int index = 0;
        while (value > 0.35f)
        {
            value -= 0.35f;
            index += 1;
        }
        _selectedIndex = index;
        _highlightRect->setPosition(0.35f*index, _highlightRect->getPosition().y);
    }
    void ChoiseBar::cursorDidExit(glm::vec2 pos)
    {
        _highlightRect->deactivate();
    }
    void ChoiseBar::cursorMovedInside(glm::vec2 pos)
    {
        glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
        float value = pos.x - min.x;
        int index = 0;
        while (value > 0.35f)
        {
            value -= 0.35f;
            index += 1;
        }
        _selectedIndex = index;
        _highlightRect->setPosition(0.35f*index, _highlightRect->getPosition().y);
    }
    bool ChoiseBar::handleClick(int action, glm::vec2 pos)
    {
        if (action == GLFW_PRESS)
        {
            //calculate which index was clicked.
            glm::vec3 min = _modelMatrix * glm::vec3(0, 0, 1.0);
            float value = pos.x - min.x;
            int index = 0;
            while (value > 0.35f)
            {
                value -= 0.35f;
                index += 1;
            }
            _selectedIndex = index;
            _foregroundRect->setPosition(0.35f*index, _foregroundRect->getPosition().y);
        }
        return false;
    }

    // setters
    void ChoiseBar::setPrimaryColor(glm::vec4 color)
    {
        _primaryColor = color;
        _backgroundRect->setColor(color);
    }
    void ChoiseBar::setSecondaryColor(glm::vec4 color)
    {
        _secondaryColor = color;
        _foregroundRect->setColor(color);
        _highlightRect->setColor(color);
    }
    void ChoiseBar::setSelected(int index)
    {
        if (index >= _numberOfChoises)
            return;
        _selectedIndex = index;
        _foregroundRect->setPosition(0.35f*index, _foregroundRect->getPosition().x);
    }
    int ChoiseBar::getSelected()
    {
        return _selectedIndex;
    }
}
