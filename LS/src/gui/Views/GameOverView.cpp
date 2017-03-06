#include "gui/Views/GameOverView.h"
#include "gui/Manager.h"
#include "gui/Views/MainMenuView.h"

namespace gui {
    GameOverView::GameOverView(EventManager* manager, const GameOverEvent &event) : _manager(manager), View()
    {
        _score = 0;
        _name = "GameOverView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        _victoryLabel = new gui::Label(font);
        if (event.hasWon())
        {
            _victoryLabel->addStringComponent(new StringComponentString("Success"));
        }
        else
        {
            _victoryLabel->addStringComponent(new StringComponentString("Defeat"));
        }
        _victoryLabel->setPosition(-_victoryLabel->getSize().x*0.5f, 0.4f);
        addChild(_victoryLabel);

        gui::Label *l = new gui::Label(font);
        l->addStringComponent(new StringComponentString("Score: "));
        l->addStringComponent(new StringComponentInt(&_score));
        l->setScale(0.5f);
        l->setPosition(-l->getSize().x*0.25f, 0.2f);
        addChild(l);

        _playAgainButton = new gui::Button(1.5f, 0.4f);
        _playAgainButton->addStringComponent(new StringComponentString("Play again"));
    	_playAgainButton->setPosition(-_playAgainButton->getSize().x*0.25f, -0.3f);
        _playAgainButton->setPrimaryColor(PALLETPRIMARY);
        _playAgainButton->setSecondaryColor(PALLETHIGHLIGHT);
        _playAgainButton->listen(this, &GameOverView::playAgain);
        _playAgainButton->setScale(0.5,0.5);
        addChild(_playAgainButton);

        _mainMenuButton = new gui::Button(1.5f, 0.4f);
        _mainMenuButton->addStringComponent(new StringComponentString("Menu"));
    	_mainMenuButton->setPosition(-_mainMenuButton->getSize().x*0.25f, -0.5f);
        _mainMenuButton->setPrimaryColor(PALLETPRIMARY);
        _mainMenuButton->setSecondaryColor(PALLETHIGHLIGHT);
        _mainMenuButton->listen(this, &GameOverView::gotoMainMenu);
        _mainMenuButton->setScale(0.5,0.5);
        addChild(_mainMenuButton);

        _quitButton = new gui::Button(1.5f, 0.4f);
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.7f);
        _quitButton->setPrimaryColor(PALLETPRIMARY);
        _quitButton->setSecondaryColor(PALLETHIGHLIGHT);
        _quitButton->listen(this, &GameOverView::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

    }
    GameOverView::~GameOverView()
    {
    }
    void GameOverView::onRender()
    {

    }
    void GameOverView::onUpdate(float dt)
    {

    }
    void GameOverView::initiate()
    {
        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _manager->execute(cEvent);
    }
    void GameOverView::updateText(const GameOverEvent &event)
    {
        if (event.hasWon())
        {
            _victoryLabel->updateStringComponent(0, new StringComponentString("Success"));
        }
        else
        {
            _victoryLabel->updateStringComponent(0, new StringComponentString("Defeat"));
        }
        _victoryLabel->setPosition(-_victoryLabel->getSize().x*0.5f, 0.4f);
    }
    void GameOverView::setScore(int score)
    {
        _score = score;
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
            MainMenuView *view = dynamic_cast<MainMenuView*>(_parent->setView("MainMenuView"));
            if (view == nullptr)
            {
                float f = 0.0f;
                view = new MainMenuView(_manager, &f);
                _parent->setView(view);
            }
        }
    }
    void GameOverView::playAgain(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("LoadingView"))
            {
                float f = 0.0f;
                _parent->setView(new LoadingView(_manager, &f));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
}
