#include "gui/Views/CreditsView.h"
#include "gui/Manager.h"

namespace gui
{
    CreditsView::CreditsView()
    {
        _name = "CreditsView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        gui::Label *l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Credits"));
        l->setPosition(0.0f - l->getSize().x * 0.5f, 0.75f-l->getSize().y/2.0f);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Simon Nilsson"));
        l->setPosition(-0.9f, 0.5f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Mikael Johnson"));
        l->setPosition(0.1f, 0.5f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Simon Gummesson"));
        l->setPosition(-0.9f, 0.25f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Dean Kulenovic"));
        l->setPosition(0.1f, 0.25f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Kim Enarsson"));
        l->setPosition(-0.9f, 0.f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Mattias Fredriksson"));
        l->setPosition(0.1f, 0.f-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        // l = new gui::Label(font);
        // l->addStringComponent(new StringComponentString("Music by:"));
        // l->setPosition(-0.9f, 0.1f-l->getSize().y/2.0f);
        // l->setScale(0.5);
        // addChild(l);
        //
        // l = new gui::Label(font);
        // l->addStringComponent(new StringComponentString("Libraries:"));
        // l->setPosition(-0.9f, -0.1f-l->getSize().y/2.0f);
        // l->setScale(0.5);
        // addChild(l);

        _backButton = new gui::Button(1.5f, 0.4f);
        _backButton->addStringComponent(new StringComponentString("Back"));
    	_backButton->setPosition(-_backButton->getSize().x*0.25f, -0.8f);
        _backButton->setPrimaryColor(PALLETPRIMARY);
        _backButton->setSecondaryColor(PALLETHIGHLIGHT);
        _backButton->listen(this, &CreditsView::back);
        _backButton->setScale(0.5,0.5);
        addChild(_backButton);
    }
    CreditsView::~CreditsView()
    {

    }

    void CreditsView::back(int action)
    {
        if (action == GLFW_PRESS)
        {
            View *view = _parent->resumeView("MainMenuView");
            if (view == nullptr)
            {
                std::cout << "Should already exist! Not possible in PauseView.cpp" << std::endl;
            }
        }
    }
}
