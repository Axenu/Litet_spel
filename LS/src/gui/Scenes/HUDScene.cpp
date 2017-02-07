#include "gui/Scenes/HUDScene.h"
#include "gui/Manager.h"

namespace gui {
    HUDScene::HUDScene(EventManager* manager, float* fps) : _manager(manager), _fps(fps), Scene()
    {
        _name = "HUDScene";

        _font = new gui::Font("Resources/fonts/arial");
        gui::Label *l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("FPS: "));
        l->addStringComponent(new StringComponentFloat(_fps));
        l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        gui::Rectangle *rect = new Rectangle(0.015f, 0.02f);
        rect->setPosition(-0.0075f, -0.01f);
        glm::vec4 color(0, 0, 0, 1);
        rect->setColor(color);
        addChild(rect);

        _manager->listen(this, &HUDScene::gameStarted);
        _manager->listen(this, &HUDScene::gameOver);
        // GameOverEvent event(true, 1000);
        // _manager->execute(event);

        //temp for ending game
        // gui::Button *b = new gui::Button();
        // b->addStringComponent(new StringComponentString("End"));
    	// b->setPosition(-b->getSize().x*0.25f, 0.0f);
        // b->listen(this, &HUDScene::endGame);
        // b->setScale(0.5,0.5);
        // addChild(b);
    }
    HUDScene::~HUDScene()
    {
        delete _font;
    }
    void HUDScene::onRender()
    {

    }
    void HUDScene::onUpdate(float dt)
    {

    }
    void HUDScene::gameStarted(const GameStartedEvent &event)
    {
        std::cout << "game has started" << std::endl;
    }
    void HUDScene::gameOver(const GameOverEvent &event)
    {
        std::cout << "GAME OVER!" << std::endl;
        if (!_parent->setScene("GameOverScene"))
        {
            std::cout << "scene not found \n Creating new..." << std::endl;
            _parent->setScene(new GameOverScene(_manager, event));
        }
        // _parent->setScene(new HUDScene(_manager, _fps));
        // ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
        // _manager->execute(event);
    }
    // void HUDScene::endGame(int action)
    // {
    //     if (action == GLFW_RELEASE)
    //     {
    //         GameOverEvent event(false, 1000);
    //         _manager->execute(event);
    //     }
    // }
}
