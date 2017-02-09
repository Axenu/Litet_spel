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
            if (!_parent->setView("HUDView"))
            {
                // std::cout << "scene not found \n Creating new..." << std::endl;
                _parent->setView(new HUDView(_manager, _fps));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
    MainMenuView::MainMenuView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
    {
        _name = "MainMenuView";

        _font = new gui::Font("Resources/fonts/arial");
        gui::Label *l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("FPS: "));
        l->addStringComponent(new StringComponentFloat(_fps));
        l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        _quitButton = new gui::Button();
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, 0.0f);
        _quitButton->listen(this, &MainMenuView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

        _startButton = new gui::Button();
        _startButton->addStringComponent(new StringComponentString("Start game"));
    	_startButton->setPosition(-_startButton->getSize().x*0.25f, 0.4f);
        _startButton->listen(this, &MainMenuView::StartGame);
        _startButton->setScale(0.5,0.5);
        addChild(_startButton);
    }
    MainMenuView::~MainMenuView()
    {
        delete _font;
        // std::cout << "gui scene destructor" << std::endl;
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
