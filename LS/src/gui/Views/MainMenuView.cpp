#include "gui/Views/MainMenuView.h"
#include "gui/Views/SettingsView.h"
#include "gui/Manager.h"

namespace gui
{
    void MainMenuView::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _eventManager->execute(event);
        }
    }
    void MainMenuView::StartGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("LoadingView"))
            {
                _parent->setView(new LoadingView(_eventManager, _fps));
            }

            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _eventManager->execute(event);
        }
    }
    void MainMenuView::Credits(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("CreditsView"))
            {
                _parent->setView(new CreditsView());
            }
        }
    }
    void MainMenuView::Settings(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("SettingsView"))
            {
                _parent->setView(new SettingsView(_eventManager));
            }
        }
    }
    MainMenuView::MainMenuView(EventManager* manager, float* fps) : _eventManager(manager), _fps(fps), View()
    {
        _name = "MainMenuView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        gui::Label *l = new Label(font);
        l->addStringComponent(new StringComponentString(GAMENAME));
        l->setPosition(0.0f - l->getSize().x * 0.5f, 0.8f-l->getSize().y/2.0f);
        addChild(l);

        if (Config::showFPS)
        {
            l = new gui::Label(font);
            l->addStringComponent(new StringComponentString("FPS: "));
            l->addStringComponent(new StringComponentFloat(_fps));
            l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
            l->setScale(0.5);
            addChild(l);
        }

        _startButton = new gui::Button(1.5f, 0.4f);
        _startButton->addStringComponent(new StringComponentString("Start game"));
    	_startButton->setPosition(-_startButton->getSize().x*0.25f, 0.3f);
        _startButton->setPrimaryColor(PALLETPRIMARY);
        _startButton->setSecondaryColor(PALLETHIGHLIGHT);
        _startButton->listen(this, &MainMenuView::StartGame);
        _startButton->setScale(0.5,0.5);
        addChild(_startButton);

        _settingsButton = new gui::Button(1.5f, 0.4f);
        _settingsButton->addStringComponent(new StringComponentString("Settings"));
    	_settingsButton->setPosition(-_startButton->getSize().x*0.25f, 0.1f);
        _settingsButton->setPrimaryColor(PALLETPRIMARY);
        _settingsButton->setSecondaryColor(PALLETHIGHLIGHT);
        _settingsButton->listen(this, &MainMenuView::Settings);
        _settingsButton->setScale(0.5,0.5);
        addChild(_settingsButton);

        _creditsButton = new gui::Button(1.5f, 0.4f);
        _creditsButton->addStringComponent(new StringComponentString("Credits"));
    	_creditsButton->setPosition(-_startButton->getSize().x*0.25f, -0.1f);
        _creditsButton->setPrimaryColor(PALLETPRIMARY);
        _creditsButton->setSecondaryColor(PALLETHIGHLIGHT);
        _creditsButton->listen(this, &MainMenuView::Credits);
        _creditsButton->setScale(0.5,0.5);
        addChild(_creditsButton);

        _quitButton = new gui::Button(1.5f, 0.4f);
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.3f);
        _quitButton->setPrimaryColor(PALLETPRIMARY);
        _quitButton->setSecondaryColor(PALLETHIGHLIGHT);
        _quitButton->listen(this, &MainMenuView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

    }
    MainMenuView::~MainMenuView()
    {
    }
    void MainMenuView::onRender(float dt)
    {

    }
    void MainMenuView::onUpdate(float dt)
    {

    }
    void MainMenuView::initiate()
    {
        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _eventManager->execute(cEvent);
    }
}
