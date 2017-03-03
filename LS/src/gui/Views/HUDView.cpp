#include "gui/Views/HUDView.h"
#include "gui/Manager.h"
#include "gui/Views/PauseView.h"

namespace gui {
    HUDView::HUDView(EventManager* manager, float* fps) : _eventManager(manager), _fps(fps), View()
    {
        _name = "HUDView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        if (sic::debug)
        {
            gui::Label *l = new gui::Label(font);
            l->addStringComponent(new StringComponentString("FPS: "));
            l->addStringComponent(new StringComponentFloat(_fps));
            l->setPosition(-1.0f, 0.8f-l->getSize().y/2.0f);
            l->setScale(0.5);
            addChild(l);
        }

        _tipDisplay = new gui::Label(font);
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
        //score background
        _scoreBackground = new Rectangle(0.5f, 0.15f);
        _scoreBackground->setPosition(-1.f, 0.85f);
        color = PALLETPRIMARY;
        color.w = 0.6f;
        _scoreBackground->setColor(color);
        addChild(_scoreBackground);
        //Display player score
        _scoreLabel = new gui::Label(font);
        _scoreLabel->addStringComponent(new StringComponentString("Score: "));
        _scoreLabel->addStringComponent(new StringComponentString(""));
        _scoreLabel->setPosition(-0.98f, 0.97f-_scoreLabel->getSize().y*0.5f);
        _scoreLabel->setScale(0.5);
        _scoreLabel->setZ(2);
        addChild(_scoreLabel);

        //progressbars for displaying soundlevel and lightlevel
        _soundPB = new ProgressBar(0.5f, 0.1f);
        _soundPB->setPrimaryColor(PALLETPRIMARY);
        _soundPB->setSecondaryColor(PALLETHIGHLIGHT);
        _soundPB->setPosition(0.1f, -0.95f);
        _soundPB->setInverted(true);
        addChild(_soundPB);

        _lightPB = new ProgressBar(0.5f, 0.1f);
        _lightPB->setPrimaryColor(PALLETPRIMARY);
        _lightPB->setSecondaryColor(PALLETHIGHLIGHT);
        _lightPB->setPosition(-0.6f, -0.95f);
        addChild(_lightPB);

        //labels describing the progressbars
        _lightLabel = new Label(font);
        _lightLabel->addStringComponent(new StringComponentString("light:"));
        _lightLabel->setScale(0.25);
        _lightLabel->setPosition(-0.6f, -0.83f);
        addChild(_lightLabel);

        _soundLabel = new Label(font);
        _soundLabel->addStringComponent(new StringComponentString("sound:"));
        _soundLabel->setScale(0.25);
        _soundLabel->setPosition(0.6f - _soundLabel->getTextWidth(), -0.83f);
        addChild(_soundLabel);

        //Grenade information
        _grenadeCountLabel = new Label(font);
        _grenadeCountLabel->addStringComponent(new StringComponentString("2"));
        _grenadeCountLabel->setScale(0.75);
        _grenadeCountLabel->setPosition(0 - _grenadeCountLabel->getTextWidth()*0.5f, -0.93f);
        addChild(_grenadeCountLabel);

		_grenadeCooldownCounter = new Label(font);
		_grenadeCooldownCounter->addStringComponent(new StringComponentString("2.000"));
        _grenadeCooldownCounter->addStringComponent(new StringComponentString(" s"));
		_grenadeCooldownCounter->setScale(0.25);
		_grenadeCooldownCounter->setPosition(0 - _grenadeCooldownCounter->getTextWidth()*0.5f, -0.82f);
		addChild(_grenadeCooldownCounter);

        _grenadeLabel = new Label(font);
        _grenadeLabel->addStringComponent(new StringComponentString("grenades"));
        _grenadeLabel->setScale(0.25);
        _grenadeLabel->setPosition(0 - _grenadeLabel->getTextWidth()*0.5f, -0.98f);
        addChild(_grenadeLabel);

        //Guard Vision
        _guardVisionPB = new ProgressBar(1.0f, 0.2f);
        _guardVisionPB->setPrimaryColor(PALLETPRIMARY);
        _guardVisionPB->setSecondaryColor(PALLETHIGHLIGHT);
        _guardVisionPB->setPosition(-0.5f, -0.95f);
        _guardVisionPB->setValue(1.0f);
        _guardVisionPB->deactivate();
        addChild(_guardVisionPB);

        _guardVisionLabel = new Label(font);
		_guardVisionLabel->addStringComponent(new StringComponentFloatConst(0.0f, 3));
		_guardVisionLabel->addStringComponent(new StringComponentString("s"));
		_guardVisionLabel->setScale(0.5);
		_guardVisionLabel->setPosition(0 - _guardVisionLabel->getTextWidth()*0.5f, -0.90f);
        _guardVisionLabel->setZ(5);
        _guardVisionLabel->deactivate();
		addChild(_guardVisionLabel);

        _eventManager->listen(this, &HUDView::switchToGuardVision);
        _eventManager->listen(this, &HUDView::gameOver);
        _eventManager->listen(this, &HUDView::exitSquareTrigger);
		_eventManager->listen(this, &HUDView::canClimb);
		_eventManager->listen(this, &HUDView::guardAlert);
        _eventManager->listen(this, &HUDView::KeyboardPressed);
        srand(time(NULL));
        _isActive = true;

        // Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 25.f, 70.f);
    	// setting._renderSetting._textureSetup[2] = GL_RGBA;
        // _g = std::unique_ptr<TestGame>(new TestGame(setting, _eventManager));

    }
    HUDView::~HUDView()
    {
        _eventManager->unlisten(this, &HUDView::switchToGuardVision);
        _eventManager->unlisten(this, &HUDView::gameOver);
        _eventManager->unlisten(this, &HUDView::exitSquareTrigger);
		_eventManager->unlisten(this, &HUDView::canClimb);
        _eventManager->unlisten(this, &HUDView::guardAlert);
        _eventManager->unlisten(this, &HUDView::KeyboardPressed);
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
        //make noise more living by adding a small random value
        float randomNoise = (rand() % 100) * 0.0002f + -0.01f;
		_soundPB->setValue(_game->getCharacter()->getNoise() + randomNoise);
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
    void HUDView::pauseView()
    {
        _game->getCharacter()->pause();
        _isActive = false;
    }
    void HUDView::resumeView()
    {
        _game->getCharacter()->resume();
        cursorModeChangeEvent event(GLFW_CURSOR_DISABLED);
        _eventManager->execute(event);
        _isActive = true;
    }
    void HUDView::initiate()
    {
        //init game
        Setting setting(_parent->getWindowWidth(), _parent->getWindowHeight(), 3, 0.1f, 25.f, 70.f);
    	setting._renderSetting._textureSetup[2] = GL_RGBA; //Specular = RGBA buffer

    	_game = std::unique_ptr<TestGame>(new TestGame(setting, _eventManager));

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
        _isActive = true;

    }
    void HUDView::switchToGuardVision(const GuardVisionEvent &event)
    {
        if (event._active)
        {
            _scoreLabel->deactivate();
            _soundPB->deactivate();
            _lightPB->deactivate();
            _grenadeCountLabel->deactivate();
            _grenadeCooldownCounter->deactivate();
            _scoreBackground->deactivate();
            _lightLabel->deactivate();
            _soundLabel->deactivate();
            _grenadeLabel->deactivate();
            _guardVisionPB->activate();
            _guardVisionLabel->activate();
            _guardVisionPB->setValue(event._duration);
            _guardVisionLabel->updateStringComponent(0, new StringComponentFloatConst(event._duration * 5.0f, 2));
            _guardVisionLabel->setPosition(0 - _guardVisionLabel->getTextWidth()*0.5f, -0.90f);
        }
        else
        {
            _scoreLabel->activate();
            _soundPB->activate();
            _lightPB->activate();
            _grenadeCountLabel->activate();
            _grenadeCooldownCounter->activate();
            _scoreBackground->activate();
            _lightLabel->activate();
            _soundLabel->activate();
            _grenadeLabel->activate();
            _guardVisionPB->deactivate();
            _guardVisionLabel->deactivate();
        }
    }
    void HUDView::KeyboardPressed(const KeyboardEvent &event)
    {
        if (event.getKey() == GLFW_KEY_ESCAPE)
        {
            if (event.getAction() == GLFW_PRESS)
            {
                if (_isActive)
                {
                    //switch scene to pause menu
                    View *view = _parent->setView("PauseView");
                    if (view == nullptr)
                    {
                        view = new PauseView(_eventManager);
                        _parent->setView(view);
                    }
                }
                else
                {
                    //switch scene to this
                    View *view = _parent->resumeView("HUDView");
                    if (view == nullptr)
                    {
                        std::cout << "Should already exist! Not possible in HUDView.cpp" << std::endl;
                    }
                }
            }
        }
    }
    //Handle game ended events
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
    }
    //Display information when player reaches exit
    void HUDView::exitSquareTrigger(const CharacterSquareEvent &event)
    {
        if (event._square._grid == gridType::exiting)
        {
            if (!_isAtExit)
            {
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
                _tipDisplay->deactivate();
                _isAtExit = false;
            }
        }
    }
    //Display information when player can climb
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
    //Display guard alert sprite
    void HUDView::guardAlert(const GuardAlertEvent &event)
    {
        bool found = false;
        TexturedRectangle *alert = nullptr;
        // test if there already exist an alert for this guard based on id
        for (size_t i = 0; i < _alerts.size(); i++)
        {
            if (_alerts[i]._id == event._id)
            {
                //store result
                alert = _alerts[i]._rect;
                found = true;
                break;
            }
        }
        if (!found)
        {
            //Create new alert if none was found
            alert = new TexturedRectangle(0.1f, 0.075f, "Resources/textures/GuardAlertV2.png");
            glm::vec4 color(0.92578125f, 0.26171875f, 0.21484375f, event._detection);
            alert->setColor(color);
            alert->setZ(4);
            addChild(alert);
            AlertStruct a;
            a._id = event._id;
            a._rect = alert;
            a._detection = event._detection;
            _alerts.push_back(a);
        }
        if (event._detection > 0.0f)
        {
            //calculate position of the alert
            glm::vec4 pos = glm::vec4(event._position.x, 1.3f, event._position.z, 1.0f);
            pos = _game->getScene().getCamera().VPMatrix * pos;
            glm::vec3 posS = glm::vec3(pos.x/pos.w, pos.y/pos.w, pos.z);
            if (posS.z < -0.2f)
            {
                posS.x = -posS.x;
                if (posS.x < 1.0f || posS.x > -1.0f)
                {
                    if (posS.y > 0.0f)
                    {
                        posS.y *= -1;
                    }
                    float length = std::sqrt(posS.x*posS.x+posS.y*posS.y);
                    posS.x /= length;
                    posS.y /= length;
                }
            }
            posS.x = std::min(posS.x, 1.0f - alert->getSize().x * 0.5f);
            posS.x = std::max(posS.x, -1.0f + alert->getSize().x * 0.5f);
            posS.y = std::min(posS.y, 1.0f - alert->getSize().y);
            posS.y = std::max(posS.y, -1.0f);
            alert->setPosition(posS.x - alert->getSize().x*0.5f, posS.y);
            alert->activate();
            //set opacity to indicate level of detection
            alert->setOpacity(event._detection);
            alert->update(0.0f);
        }
        else
        {
            //if the guard no longer notices the player, hide it.
            alert->deactivate();
        }
    }
}
