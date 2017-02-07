#include "gui/Scenes/MainMenuScene.h"
#include "gui/Manager.h"

namespace gui
{
    void MainMenuScene::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _manager->execute(event);
        }
    }
    void MainMenuScene::StartGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            if (!_parent->setScene("HUDScene"))
            {
                std::cout << "scene not found \n Creating new..." << std::endl;
                _parent->setScene(new HUDScene(_manager, _fps));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
    MainMenuScene::MainMenuScene(EventManager* manager, float* fps) : _manager(manager), _fps(fps), Scene()
    {
        _name = "MainMenu";

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
        _quitButton->listen<gui::MainMenuScene>(this, &MainMenuScene::QuitGame);
        _quitButton->setScale(0.5,0.5);
        addChild(_quitButton);

        _startButton = new gui::Button();
        _startButton->addStringComponent(new StringComponentString("Start game"));
    	_startButton->setPosition(-_startButton->getSize().x*0.25f, 0.4f);
        _startButton->listen<gui::MainMenuScene>(this, &MainMenuScene::StartGame);
        _startButton->setScale(0.5,0.5);
        addChild(_startButton);

        //create local buttons.
        //bind to functions
    }
    MainMenuScene::~MainMenuScene()
    {
        delete _font;
        // std::cout << "gui scene destructor" << std::endl;
    }
    void MainMenuScene::onRender()
    {

    }
    void MainMenuScene::onUpdate(float dt)
    {

    }
}
