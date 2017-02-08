#include "gui/Views/GameOverView.h"
#include "gui/Manager.h"

namespace gui {
    GameOverView::GameOverView(EventManager* manager, const GameOverEvent &event) : _manager(manager), View()
    {
        _name = "GameOverScene";

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
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.3f);
        _quitButton->listen(this, &GameOverView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);
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
}
