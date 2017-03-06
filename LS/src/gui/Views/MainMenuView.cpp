#include "gui/Views/MainMenuView.h"
#include "gui/Manager.h"

namespace gui
{
    void MainMenuView::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _manager->execute(event);
        }
    }
    void MainMenuView::StartGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("LoadingView"))
            {
                // std::cout << "scene not found \n Creating new..." << std::endl;
                _parent->setView(new LoadingView(_manager, _fps));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
    MainMenuView::MainMenuView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
    {
        _name = "MainMenuView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        if (sic::debug)
        {
            gui::Label *l = new gui::Label(font);
            l->addStringComponent(new StringComponentString("FPS: "));
            l->addStringComponent(new StringComponentFloat(_fps));
            l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
            l->setScale(0.5);
            addChild(l);
        }

        _startButton = new gui::Button(1.5f, 0.4f);
        _startButton->addStringComponent(new StringComponentString("Start game"));
    	_startButton->setPosition(-_startButton->getSize().x*0.25f, 0.4f);
        _startButton->setPrimaryColor(PALLETPRIMARY);
        _startButton->setSecondaryColor(PALLETHIGHLIGHT);
        _startButton->listen(this, &MainMenuView::StartGame);
        _startButton->setScale(0.5,0.5);
        addChild(_startButton);

        _settingsButton = new gui::Button(1.5f, 0.4f);
        _settingsButton->addStringComponent(new StringComponentString("Settings"));
    	_settingsButton->setPosition(-_startButton->getSize().x*0.25f, 0.2f);
        _settingsButton->setPrimaryColor(PALLETPRIMARY);
        _settingsButton->setSecondaryColor(PALLETHIGHLIGHT);
        // _startButton->listen(this, &MainMenuView::StartGame);
        _settingsButton->setScale(0.5,0.5);
        addChild(_settingsButton);

        _creditsButton = new gui::Button(1.5f, 0.4f);
        _creditsButton->addStringComponent(new StringComponentString("Credits"));
    	_creditsButton->setPosition(-_startButton->getSize().x*0.25f, 0.0f);
        _creditsButton->setPrimaryColor(PALLETPRIMARY);
        _creditsButton->setSecondaryColor(PALLETHIGHLIGHT);
        // _startButton->listen(this, &MainMenuView::StartGame);
        _creditsButton->setScale(0.5,0.5);
        addChild(_creditsButton);

        _quitButton = new gui::Button(1.5f, 0.4f);
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.2f);
        _quitButton->setPrimaryColor(PALLETPRIMARY);
        _quitButton->setSecondaryColor(PALLETHIGHLIGHT);
        _quitButton->listen(this, &MainMenuView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

    }
    MainMenuView::~MainMenuView()
    {
    }
    void MainMenuView::onRender()
    {

    }
    void MainMenuView::onUpdate(float dt)
    {

    }
    void MainMenuView::initiate()
    {
        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _manager->execute(cEvent);
    }
}
