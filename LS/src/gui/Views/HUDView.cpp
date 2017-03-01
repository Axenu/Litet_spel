#include "gui/Views/HUDView.h"
#include "gui/Manager.h"

namespace gui {
    HUDView::HUDView(EventManager* manager, float* fps) : _eventManager(manager), _fps(fps), View()
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

        _soundPB = new ProgressBar(0.5f, 0.1f);
        _soundPB->setPrimaryColor(PALLETPRIMARY);
        _soundPB->setSecondaryColor(PALLETHIGHLIGHT);
        _soundPB->setPosition(0.1, -0.95);
        _soundPB->setInverted(true);
        addChild(_soundPB);
        _lightPB = new ProgressBar(0.5f, 0.1f);
        _lightPB->setPrimaryColor(PALLETPRIMARY);
        _lightPB->setSecondaryColor(PALLETHIGHLIGHT);
        _lightPB->setPosition(-0.6, -0.95);
        addChild(_lightPB);

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

		_grenadeCooldownCounter = new Label(_font);
		_grenadeCooldownCounter->addStringComponent(new StringComponentString("2.000"));
        _grenadeCooldownCounter->addStringComponent(new StringComponentString(" s"));
		_grenadeCooldownCounter->setScale(0.25);
		_grenadeCooldownCounter->setPosition(0 - _grenadeCooldownCounter->getTextWidth()*0.5f, -0.82f);
		addChild(_grenadeCooldownCounter);

        la = new Label(_font);
        la->addStringComponent(new StringComponentString("grenades"));
        la->setScale(0.25);
        la->setPosition(0 - la->getTextWidth()*0.5f, -0.98f);
        addChild(la);

        _eventManager->listen(this, &HUDView::gameStarted);
        _eventManager->listen(this, &HUDView::gameOver);
        _eventManager->listen(this, &HUDView::exitSquareTrigger);
		_eventManager->listen(this, &HUDView::canClimb);
    }
    HUDView::~HUDView()
    {
        _eventManager->unlisten(this, &HUDView::gameStarted);
        _eventManager->unlisten(this, &HUDView::gameOver);
        _eventManager->unlisten(this, &HUDView::exitSquareTrigger);
		_eventManager->unlisten(this, &HUDView::canClimb);
        delete _font;
        delete _game;
    }
    void HUDView::onRender()
    {
        _game->draw();
    }
    void HUDView::onUpdate(float dt)
    {
        _grenadeCountLabel->setPosition(0 - _grenadeCountLabel->getTextWidth()*0.5f, -0.93f);
        _game->update(dt);
        _lightPB->setValue(_game->getCharacter()->getLightAtPosition());
        if (*(_game->getCharacter()->getGrenadeCooldownTimer()) > 0.0f)
        {
            _grenadeCooldownCounter->activate();
            _grenadeCooldownCounter->setPosition(0 - _grenadeCooldownCounter->getTextWidth()*0.5f, -0.82f);
        }
        else
        {
            _grenadeCooldownCounter->deactivate();
        }
    }
    void HUDView::initiate()
    {
        if (_game != nullptr)
        {
            // std::cout << "delete game" << std::endl;
            delete _game;
        }
        //init game
        Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 25.f, 70.f);
    	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

    	_game = new TestGame(setting, _eventManager);

        /* Load game
    	*/
    	_game->initiate();
        //Update score label
        _scoreLabel->updateStringComponent(1, new StringComponentInt(_game->getCharacter()->getLootValuePointer()));
        //update grenade label
        _grenadeCountLabel->updateStringComponent(0, new StringComponentInt(_game->getCharacter()->getGrenadeCountPointer()));
        //update cooldown label
		_grenadeCooldownCounter->updateStringComponent(0, new StringComponentFloat(_game->getCharacter()->getGrenadeCooldownTimer()));
        cursorModeChangeEvent event(GLFW_CURSOR_DISABLED);
        _eventManager->execute(event);

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
            view = new GameOverView(_eventManager, event);
            _parent->setView(view);
        }
        view->setScore(*_game->getCharacter()->getLootValuePointer());
        view->updateText(event);
        // delete _game;
        // _game = nullptr;
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
