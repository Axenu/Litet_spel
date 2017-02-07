#include "gui/Scenes/GameOverScene.h"
#include "gui/Manager.h"

namespace gui {
    GameOverScene::GameOverScene(EventManager* manager, const GameOverEvent &event) : _manager(manager), Scene()
    {
        _name = "GameOverScene";

        _font = new gui::Font("Resources/fonts/arial");
        gui::Label *l = new gui::Label(_font);
        if (event.hasWon())
        {
            l->addStringComponent(new StringComponentString("Success"));
        }
        else
        {
            l->addStringComponent(new StringComponentString("Defeat"));
        }
        l->setPosition(-l->getSize().x*0.5f, 0.2f);
        addChild(l);

        l = new gui::Label(_font);
        l->addStringComponent(new StringComponentString("Score: " + std::to_string(event.getScore())));
        l->setScale(0.5f);
        l->setPosition(-l->getSize().x*0.25f, 0.0f);
        addChild(l);
    }
    GameOverScene::~GameOverScene()
    {
        delete _font;
    }
    void GameOverScene::onRender()
    {

    }
    void GameOverScene::onUpdate(float dt)
    {

    }
}
