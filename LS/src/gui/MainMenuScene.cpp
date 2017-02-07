#include "gui/MainMenuScene.h"
#include "gui/Manager.h"

namespace gui
{
    void MainMenuScene::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            // std::cout << "button click" << std::endl;
            QuitGameEvent event;
            _manager->execute(event);
        }
    }
    void MainMenuScene::StartGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            std::cout << "Start game clicked" << std::endl;
            // QuitGameEvent event;
            // _manager->execute(event);
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

        // _quitButton = new gui::Button();
        // _quitButton->addStringComponent(new StringComponentString("Quit"));
    	// _quitButton->setPosition(-_quitButton->getSize().x*0.25f, 0.0f);
        // _quitButton->listen<gui::MainMenuScene>(this, &MainMenuScene::QuitGame);
        // _quitButton->setScale(0.5,0.5);
        // addChild(_quitButton);
        //
        // _startButton = new gui::Button();
        // _startButton->addStringComponent(new StringComponentString("Start game"));
    	// _startButton->setPosition(-_startButton->getSize().x*0.25f, 0.4f);
        // _startButton->listen<gui::MainMenuScene>(this, &MainMenuScene::StartGame);
        // _startButton->setScale(0.5,0.5);
        // addChild(_startButton);

        gui::Rectangle *rect = new Rectangle(0.015f, 0.02f);
        rect->setPosition(-0.0075f, -0.01f);
        glm::vec4 color(0, 0, 0, 1);
        rect->setColor(color);
        addChild(rect);

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
