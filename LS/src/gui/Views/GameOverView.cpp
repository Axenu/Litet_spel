#include "gui/Views/GameOverView.h"
#include "gui/Manager.h"
#include "gui/Views/MainMenuView.h"

namespace gui {
    GameOverView::GameOverView(EventManager* manager, const GameOverEvent &event) : _manager(manager), View()
    {
        _name = "GameOverView";

        _font = new gui::Font("Resources/fonts/arial");
        gui::Label *l = new gui::Label(_font);
        if (event.hasWon())
        {
            l->addStringComponent(new StringComponentString("Success"));
        }
        else
        {
            l->addStringComponent(new StringComponentString("Defeat"));
        }
        l->setPosition(-l->getSize().x*0.5f, 0.2f);
        addChild(l);

        l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("Score: " + std::to_string(event.getScore())));
        l->setScale(0.5f);
        l->setPosition(-l->getSize().x*0.25f, 0.0f);
        addChild(l);

        _quitButton = new gui::Button();
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.6f);
        _quitButton->listen(this, &GameOverView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

        _mainMenuButton = new gui::Button();
        _mainMenuButton->addStringComponent(new StringComponentString("Menu"));
    	_mainMenuButton->setPosition(-_mainMenuButton->getSize().x*0.25f, -0.3f);
        _mainMenuButton->listen(this, &GameOverView::gotoMainMenu);
        _mainMenuButton->setScale(0.5,0.5);
        addChild(_mainMenuButton);

        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _manager->execute(cEvent);
    }
    GameOverView::~GameOverView()
    {
        delete _font;
    }
    void GameOverView::onRender()
    {

    }
    void GameOverView::onUpdate(float dt)
    {

    }
    void GameOverView::initiate()
    {
        
    }
    void GameOverView::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _manager->execute(event);
        }
    }
    void GameOverView::gotoMainMenu(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("MainMenuView"))
            {
                std::cout << "scene not found \n Creating new... (Should already exist)" << std::endl;
                float f = 0.0f;
                _parent->setView(new MainMenuView(_manager, &f));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
}
