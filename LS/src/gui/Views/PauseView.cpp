#include "gui/Views/PauseView.h"
#include "gui/Views/MainMenuView.h"
#include "gui/Views/HUDView.h"
#include "gui/Manager.h"

namespace gui
{
    void PauseView::quitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _eventManager->execute(event);
        }
    }
    void PauseView::mainMenu(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setView("MainMenuView"))
            {
                std::cout << "ERROR: Could not resume game, no MainMenuView found!" << std::endl;
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _eventManager->execute(event);
        }
    }
    void PauseView::resumeGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->resumeView("HUDView"))
            {
                std::cout << "ERROR: Could not resume game, no HUDView found!" << std::endl;
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _eventManager->execute(event);
        }
    }
    PauseView::PauseView(EventManager* manager) : _eventManager(manager), View()
    {
        _name = "PauseView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");

        Label *l = new Label(font);
        l->addStringComponent(new StringComponentString("Paused"));
        l->setPosition(-l->getSize().x * 0.5f, 0.5f);
        addChild(l);

        _resumeButton = new gui::Button(1.5f, 0.4f);
        _resumeButton->addStringComponent(new StringComponentString("Resume"));
    	_resumeButton->setPosition(-_resumeButton->getSize().x*0.25f, 0.1f);
        _resumeButton->setPrimaryColor(PALLETPRIMARY);
        _resumeButton->setSecondaryColor(PALLETHIGHLIGHT);
        _resumeButton->listen(this, &PauseView::resumeGame);
        _resumeButton->setScale(0.5,0.5);
        addChild(_resumeButton);

        _mainMenuButton = new gui::Button(1.5f, 0.4f);
        _mainMenuButton->addStringComponent(new StringComponentString("Main menu"));
    	_mainMenuButton->setPosition(-_mainMenuButton->getSize().x*0.25f, -0.1f);
        _mainMenuButton->setPrimaryColor(PALLETPRIMARY);
        _mainMenuButton->setSecondaryColor(PALLETHIGHLIGHT);
        _mainMenuButton->listen(this, &PauseView::mainMenu);
        _mainMenuButton->setScale(0.5,0.5);
        addChild(_mainMenuButton);

        _quitButton = new gui::Button(1.5f, 0.4f);
        _quitButton->addStringComponent(new StringComponentString("Quit"));
    	_quitButton->setPosition(-_quitButton->getSize().x*0.25f, -0.3f);
        _quitButton->setPrimaryColor(PALLETPRIMARY);
        _quitButton->setSecondaryColor(PALLETHIGHLIGHT);
        _quitButton->listen(this, &PauseView::quitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

    }
    PauseView::~PauseView()
    {
    }
    void PauseView::onRender()
    {

    }
    void PauseView::onUpdate(float dt)
    {

    }
    void PauseView::initiate()
    {
        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _eventManager->execute(cEvent);
    }
}
