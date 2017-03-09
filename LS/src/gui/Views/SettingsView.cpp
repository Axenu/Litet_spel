#include "gui/Views/SettingsView.h"
#include "gui/Manager.h"

namespace gui
{
    SettingsView::SettingsView(EventManager *em) : View(), _eventManager(em)
    {
        _name = "SettingsView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        gui::Label *l = new Label(font);
        l->addStringComponent(new StringComponentString("Settings"));
        l->setPosition(0.0f - l->getSize().x * 0.5f, 0.9f-l->getSize().y/2.0f);
        addChild(l);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Sound:"));
        l->setPosition(-0.75f, 0.7f);
        l->setScale(0.5f);
        addChild(l);

        _soundPB = new ProgressBar(0.5f, 0.1f);
        _soundPB->setPrimaryColor(PALLETPRIMARY);
        _soundPB->setSecondaryColor(PALLETHIGHLIGHT);
        _soundPB->setPosition(-0.75, 0.58f);
        _soundPB->setReactive(true);
        _soundPB->setValue(Config::masterVolume);
        // _soundPB->setInverted(true);
        addChild(_soundPB);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Sound enabled:"));
        l->setPosition(-0.2f, 0.60f);
        l->setScale(0.4f);
        addChild(l);

        _soundCheckbox = new Checkbox(0.07f, 0.1f);
        _soundCheckbox->setPrimaryColor(PALLETPRIMARY);
        _soundCheckbox->setSecondaryColor(PALLETHIGHLIGHT);
        _soundCheckbox->setPosition(0.32f, 0.58f);
        _soundCheckbox->setSelected(Config::hasSound);
        addChild(_soundCheckbox);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Resolution:"));
        l->setPosition(-0.75f, 0.45f);
        l->setScale(0.5f);
        addChild(l);

        std::string choises[] = {"480x320", "1280x720", "1920x1080", "2560x1440"};

        _resolutionBar = new ChoiseBar(choises, 4);
        _resolutionBar->setPrimaryColor(PALLETPRIMARY);
        _resolutionBar->setSecondaryColor(PALLETHIGHLIGHT);
        _resolutionBar->setPosition(-0.75f, 0.3f);
        if (Config::resolution.x == 480 && Config::resolution.y == 320)
            _resolutionBar->setSelected(0);
        else if (Config::resolution.x == 1280 && Config::resolution.y == 720)
            _resolutionBar->setSelected(1);
        else if (Config::resolution.x == 1920 && Config::resolution.y == 1080)
            _resolutionBar->setSelected(2);
        else if (Config::resolution.x == 2560 && Config::resolution.y == 1440)
            _resolutionBar->setSelected(3);
        addChild(_resolutionBar);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("borederless:"));
        l->setPosition(-0.75f, 0.18f);
        l->setScale(0.4f);
        addChild(l);

        _borderlessCheckbox = new Checkbox(0.07f, 0.1f);
        _borderlessCheckbox->setPrimaryColor(PALLETPRIMARY);
        _borderlessCheckbox->setSecondaryColor(PALLETHIGHLIGHT);
        _borderlessCheckbox->setPosition(-0.33f, 0.16f);
        _borderlessCheckbox->setSelected(Config::borderLess);
        addChild(_borderlessCheckbox);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("Shadow quality:"));
        l->setPosition(-0.75f, 0.0f);
        l->setScale(0.5f);
        addChild(l);

        std::string shadowChoises[] = {"Off", "Low", "Medium", "High"};
        _shadowBar = new ChoiseBar(shadowChoises, 4);
        _shadowBar->setPrimaryColor(PALLETPRIMARY);
        _shadowBar->setSecondaryColor(PALLETHIGHLIGHT);
        _shadowBar->setPosition(-0.75f, -0.15f);
        if (!Config::hasShadowMap)
            _shadowBar->setSelected(0);
        else if (Config::shadowMapSize == 256)
            _shadowBar->setSelected(1);
        else if (Config::shadowMapSize == 512)
            _shadowBar->setSelected(2);
        else if (Config::shadowMapSize == 1024)
            _shadowBar->setSelected(3);
        addChild(_shadowBar);

        l = new Label(font);
        l->addStringComponent(new StringComponentString("fps:"));
        l->setPosition(-0.75f, -0.27f);
        l->setScale(0.4f);
        addChild(l);

        _fpsCheckbox = new Checkbox(0.07f, 0.1f);
        _fpsCheckbox->setPrimaryColor(PALLETPRIMARY);
        _fpsCheckbox->setSecondaryColor(PALLETHIGHLIGHT);
        _fpsCheckbox->setPosition(-0.72f+l->getTextWidth(), -0.29f);
        _fpsCheckbox->setSelected(Config::showFPS);
        addChild(_fpsCheckbox);

        _backButton = new gui::Button(1.5f, 0.4f);
        _backButton->addStringComponent(new StringComponentString("Back"));
    	_backButton->setPosition(-_backButton->getSize().x*0.25f, -0.8f);
        _backButton->setPrimaryColor(PALLETPRIMARY);
        _backButton->setSecondaryColor(PALLETHIGHLIGHT);
        _backButton->listen(this, &SettingsView::back);
        _backButton->setScale(0.5,0.5);
        addChild(_backButton);
    }
    SettingsView::~SettingsView()
    {

    }

    void SettingsView::back(int action)
    {
        if (action == GLFW_PRESS)
        {
            //save changes
            Config::masterVolume = _soundPB->getValue();
            Config::hasSound = _soundCheckbox->getSelected();
            Config::borderLess = _borderlessCheckbox->getSelected();
            Config::showFPS = _fpsCheckbox->getSelected();
            //switch for resolution options
            float oldRes = Config::resolution.x;
            switch(_resolutionBar->getSelected())
            {
                case 0:
                    Config::resolution.x = 480;
                    Config::resolution.y = 320;
                break;
                case 1:
                    Config::resolution.x = 1280;
                    Config::resolution.y = 720;
                break;
                case 2:
                    Config::resolution.x = 1920;
                    Config::resolution.y = 1080;
                break;
                case 3:
                    Config::resolution.x = 2560;
                    Config::resolution.y = 1440;
                break;
                default:
                break;
            }
            if (oldRes != Config::resolution.x)
            {
                ResizeWindowEvent event(Config::resolution.x, Config::resolution.y);
                _eventManager->execute(event);
            }
            switch(_shadowBar->getSelected())
            {
                case 0:
                    Config::hasShadowMap = false;
                    Config::shadowMapSize = 1;
                break;
                case 1:
                    Config::hasShadowMap = true;
                    Config::shadowMapSize = 256;
                break;
                case 2:
                    Config::hasShadowMap = true;
                    Config::shadowMapSize = 512;
                break;
                case 3:
                    Config::hasShadowMap = true;
                    Config::shadowMapSize = 1024;
                break;
                default:
                break;
            }
            Config::saveConfig("resources/settings.conf");
            View *view = _parent->resumeView("MainMenuView");
            if (view == nullptr)
            {
                std::cout << "Should already exist! Not possible in PauseView.cpp" << std::endl;
            }
        }
    }
}
