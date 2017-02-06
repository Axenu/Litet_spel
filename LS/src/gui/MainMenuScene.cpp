#include "gui/MainMenuScene.h"
#include "gui/Manager.h"

namespace gui
{
    MainMenuScene::MainMenuScene(EventManager* manager) : _manager(manager), Scene()
    {
        _name = "MainMenu";
        _quitButton = new gui::Button("Quit");
    	_quitButton->setPosition(-1, -0.9);
        _quitButton->listen(this, &MainMenuScene::QuitGame);
        _quitButton->setSize(0.5,0.5);
        addChild(_quitButton);
        // _quitButton = new gui::Button("Start");
    	// _quitButton->setPosition(1, 1.3);
        // _quitButton->listen(this, &MainMenuScene::QuitGame);
        // addChild(_quitButton);

        gui::Rectangle *rect = new Rectangle(0.5, 0.5);
        glm::vec4 color(0,0.5,0,1);
        rect->setColor(color);
        addChild(rect);

        //create local buttons.
        //bind to functions
    }
    MainMenuScene::~MainMenuScene()
    {
        // std::cout << "gui scene destructor" << std::endl;
    }
    void MainMenuScene::onRender()
    {

    }
    void MainMenuScene::onUpdate(float dt)
    {

    }
    void MainMenuScene::QuitGame(int action)
    {
        if (action == GLFW_RELEASE)
        {
            QuitGameEvent event;
            _manager->execute(event);
        }
    }
}
