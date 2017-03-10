#include "gui/Views/InGameSettingsView.h"
#include "gui/Manager.h"

namespace gui
{
    InGameSettingsView::InGameSettingsView() : View()
    {
        _name = "InGameSettingsView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        gui::Label *l = new Label(font);
        l->addStringComponent(new StringComponentString("Settings"));
        l->setPosition(0.0f - l->getSize().x * 0.5f, 0.9f-l->getSize().y/2.0f);
        addChild(l);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Sound:"));
        l->setPosition(-0.25f, 0.55f);
        l->setScale(0.5f);
        addChild(l);

        _soundPB = new ProgressBar(0.5f, 0.1f);
        _soundPB->setPrimaryColor(PALLETPRIMARY);
        _soundPB->setSecondaryColor(PALLETHIGHLIGHT);
        _soundPB->setPosition(-0.25f, 0.4f);
        _soundPB->setReactive(true);
        _soundPB->setValue(Config::masterVolume);
        // _soundPB->setInverted(true);
        addChild(_soundPB);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Sound enabled:"));
        l->setPosition(-l->getTextWidth()*0.25f, 0.2f);
        l->setScale(0.4f);
        addChild(l);

        _soundCheckbox = new Checkbox(0.07f, 0.1f);
        _soundCheckbox->setPrimaryColor(PALLETPRIMARY);
        _soundCheckbox->setSecondaryColor(PALLETHIGHLIGHT);
        _soundCheckbox->setPosition(0.25f, 0.175f);
        _soundCheckbox->setSelected(Config::hasSound);
        addChild(_soundCheckbox);

        _backButton = new gui::Button(1.5f, 0.4f);
        _backButton->addStringComponent(new StringComponentString("Back"));
    	_backButton->setPosition(-_backButton->getSize().x*0.25f, -0.8f);
        _backButton->setPrimaryColor(PALLETPRIMARY);
        _backButton->setSecondaryColor(PALLETHIGHLIGHT);
        _backButton->listen(this, &InGameSettingsView::back);
        _backButton->setScale(0.5,0.5);
        addChild(_backButton);
    }
    InGameSettingsView::~InGameSettingsView()
    {

    }

    void InGameSettingsView::back(int action)
    {
        if (action == GLFW_PRESS)
        {
            //save changes
            Config::masterVolume = _soundPB->getValue();
            Config::hasSound = _soundCheckbox->getSelected();
            SoundManager::getInstance().setGlobalVolume(Config::masterVolume);
            if (!Config::hasSound)
                SoundManager::getInstance().setGlobalVolume(0.0f);
            Config::saveConfig("resources/settings.conf");
            View *view = _parent->resumeView("PauseView");
            if (view == nullptr)
            {
                std::cout << "Should already exist! Not possible in InGameSettingsView.cpp" << std::endl;
            }
        }
    }
}
