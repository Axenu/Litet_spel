#include "gui/Views/HUDView.h"
#include "gui/Manager.h"

namespace gui {
    HUDView::HUDView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
    {
        _name = "HUDView";

        _font = new gui::Font("Resources/fonts/arial");
        // gui::Label *l = new gui::Label(_font);
        // l->addStringComponent(new StringComponentString("FPS: "));
        // l->addStringComponent(new StringComponentFloat(_fps));
        // l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
        // l->setScale(0.5);
        // addChild(l);

        _tipDisplay = new gui::Label(_font);
        _tipDisplay->addStringComponent(new StringComponentString("Temp string"));
        _tipDisplay->setPosition(-_tipDisplay->getSize().x*0.25f, -0.5);
        _tipDisplay->setScale(0.5);
        _tipDisplay->deactivate();
        addChild(_tipDisplay);

        //aim
        gui::Rectangle *rect = new Rectangle(0.015f, 0.02f);
        rect->setPosition(-0.0075f, -0.01f);
        glm::vec4 color(0, 0, 0, 1);
        rect->setColor(color);
        addChild(rect);
        rect = new Rectangle(0.5f, 0.15f);
        rect->setPosition(-1.f, 0.85f);
        color = PALLETPRIMARY;
        color.w = 0.6f;
        rect->setColor(color);
        addChild(rect);

        _scoreLabel= new gui::Label(_font);
        _scoreLabel->addStringComponent(new StringComponentString("Score: "));
        _scoreLabel->addStringComponent(new StringComponentString(""));
        _scoreLabel->setPosition(-0.98f, 0.97f-_scoreLabel->getSize().y*0.5f);
        _scoreLabel->setScale(0.5);
        _scoreLabel->setZ(2);
        addChild(_scoreLabel);

        ProgressBar *pb = new ProgressBar(0.5f, 0.1f);
        pb->setPrimaryColor(PALLETPRIMARY);
        pb->setSecondaryColor(PALLETHIGHLIGHT);
        pb->setPosition(0.1, -0.95);
        pb->setInverted(true);
        addChild(pb);
        pb = new ProgressBar(0.5f, 0.1f);
        pb->setPrimaryColor(PALLETPRIMARY);
        pb->setSecondaryColor(PALLETHIGHLIGHT);
        pb->setPosition(-0.6, -0.95);
        addChild(pb);

        Label *la = new Label(_font);
        la->addStringComponent(new StringComponentString("light:"));
        la->setScale(0.25);
        la->setPosition(-0.6, -0.83f);
        addChild(la);
        la = new Label(_font);
        la->addStringComponent(new StringComponentString("sound:"));
        la->setScale(0.25);
        la->setPosition(0.6f - la->getTextWidth(), -0.83f);
        addChild(la);
        _grenadeCountLabel = new Label(_font);
        _grenadeCountLabel->addStringComponent(new StringComponentString("2"));
        _grenadeCountLabel->setScale(0.75);
        _grenadeCountLabel->setPosition(0 - _grenadeCountLabel->getTextWidth()*0.5f, -0.93f);
        addChild(_grenadeCountLabel);
        la = new Label(_font);
        la->addStringComponent(new StringComponentString("grenades"));
        la->setScale(0.25);
        la->setPosition(0 - la->getTextWidth()*0.5f, -0.98f);
        addChild(la);

        _manager->listen(this, &HUDView::gameStarted);
        _manager->listen(this, &HUDView::gameOver);
        _manager->listen(this, &HUDView::exitSquareTrigger);
		_manager->listen(this, &HUDView::canClimb);
    }
    HUDView::~HUDView()
    {
        _manager->unlisten(this, &HUDView::gameStarted);
        _manager->unlisten(this, &HUDView::gameOver);
        _manager->unlisten(this, &HUDView::exitSquareTrigger);
		_manager->unlisten(this, &HUDView::canClimb);
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
        Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 25.f, 70.f);
    	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

    	_game = new TestGame(setting, *_manager);

        /* Load game
    	*/
    	_game->initiate();
        //Update score label
        _scoreLabel->updateStringComponent(1, new StringComponentInt(_game->getCharacter()->getLootValuePointer()));
        //update grenade label
        _grenadeCountLabel->updateStringComponent(0, new StringComponentInt(_game->getCharacter()->getGrenadeCountPointer()));

        cursorModeChangeEvent event(GLFW_CURSOR_DISABLED);
        _manager->execute(event);

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
        view->updateText(event);
        delete _game;
        _game = nullptr;
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
	void HUDView::canClimb(const CanClimbEvent & event)
	{
		if (event.canClimb())
		{
			_tipDisplay->updateStringComponent(0, new StringComponentString("Press Space to Climb."));
			_tipDisplay->activate();
			_tipDisplay->setPosition(-_tipDisplay->getSize().x*0.25f, -0.5);
			_tipDisplay->update(0.0f);
		}
		else
		{
			_tipDisplay->deactivate();
		}
	}
}
