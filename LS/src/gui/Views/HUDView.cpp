#include "gui/Views/HUDView.h"
#include "gui/Manager.h"

namespace gui {
    HUDView::HUDView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
    {
        _name = "HUDView";

        _font = new gui::Font("Resources/fonts/arial");
        gui::Label *l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("FPS: "));
        l->addStringComponent(new StringComponentFloat(_fps));
        l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
        l->setScale(0.5);
        addChild(l);

        _tipDisplay = new gui::Label(_font);
        _tipDisplay->addStringComponent(new StringComponentString("Temp string"));
        // _tipDisplay->addStringComponent(new StringComponentFloat(_fps));
        _tipDisplay->setPosition(-_tipDisplay->getSize().x*0.25f, -0.5);
        _tipDisplay->setScale(0.5);
        _tipDisplay->deactivate();
        addChild(_tipDisplay);

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
        if (_game != nullptr)
        {
            std::cout << "delete game" << std::endl;
            delete _game;
        }
        //init game
        Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 100.f, 70.f);
    	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

    	_game = new TestGame(setting, *_manager);

        /* Load game
    	*/
    	_game->initiate();
        //Add score to ui
        gui::Label *l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("Score: "));
        l->addStringComponent(new StringComponentInt(_game->getCharacter()->getLootValuePointer()));
        l->setPosition(0.90f-l->getSize().x*0.5f, 1-l->getSize().y*0.5f);
        l->setScale(0.5);
        addChild(l);
    }
    void HUDView::gameStarted(const GameStartedEvent &event)
    {
        std::cout << "game has started" << std::endl;
    }
    void HUDView::gameOver(const GameOverEvent &event)
    {
        GameOverView *view = dynamic_cast<GameOverView*>(_parent->setView("GameOverView"));
        if (view == nullptr)
        {
            view = new GameOverView(_manager, event);
            _parent->setView(view);
        }
        view->setScore(*_game->getCharacter()->getLootValuePointer());
    }
    void HUDView::exitSquareTrigger(const CharacterSquareEvent &event)
    {
        if (event._square._grid == gridType::exiting)
        {
            if (!_isAtExit)
            {
                std::cout << "player entered exit" << std::endl;
                _tipDisplay->updateStringComponent(0, new StringComponentString("Press G to finish."));
                _tipDisplay->activate();
                _tipDisplay->setPosition(-_tipDisplay->getSize().x*0.25f, -0.5);
                _tipDisplay->update(0.0f);
                _isAtExit = true;
            }
        }
        else
        {
            if (_isAtExit)
            {
                std::cout << "player left exit" << std::endl;
                _tipDisplay->deactivate();
                _isAtExit = false;
            }
        }
    }
}
