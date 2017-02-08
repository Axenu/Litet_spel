#include "gui/Views/HUDView.h"
#include "gui/Manager.h"

namespace gui {
    HUDView::HUDView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
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

        _manager->listen(this, &HUDView::gameStarted);
        _manager->listen(this, &HUDView::gameOver);
        _manager->listen(this, &HUDView::exitSquareTrigger);
        cursorModeChangeEvent event(GLFW_CURSOR_DISABLED);
        _manager->execute(event);
    }
    HUDView::~HUDView()
    {
        delete _font;
        delete _game;
    }
    void HUDView::onRender()
    {
        _game->draw();
    }
    void HUDView::onUpdate(float dt)
    {
        _game->update(dt);
    }
    void HUDView::initiate()
    {
        //init game
        Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 100.f, 70.f);
    	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

    	_game = new TestGame(setting, *_manager);

        /* Load game
    	*/
    	_game->initiate();
    }
    void HUDView::gameStarted(const GameStartedEvent &event)
    {
        std::cout << "game has started" << std::endl;
    }
    void HUDView::gameOver(const GameOverEvent &event)
    {
        // std::cout << "GAME OVER!" << std::endl;
        if (!_parent->setView("GameOverView"))
        {
            // std::cout << "scene not found \n Creating new..." << std::endl;
            _parent->setView(new GameOverView(_manager, event));
        }
        // _parent->setScene(new HUDScene(_manager, _fps));
        // ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
        // _manager->execute(event);
    }
    void HUDView::exitSquareTrigger(const ExitTriggerEvent &event)
    {
        if (event.didEnter())
        {
            std::cout << "player entered exit" << std::endl;
        }
        else
        {
            std::cout << "player left exit" << std::endl;
        }
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
