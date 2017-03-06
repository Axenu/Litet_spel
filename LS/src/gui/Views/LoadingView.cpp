#include "gui/Views/LoadingView.h"
#include "gui/Manager.h"

namespace gui
{
    LoadingView::LoadingView(EventManager* manager, float* fps) : _manager(manager), _fps(fps), View()
    {
        _name = "LoadingView";

        gui::Font *font = Factory::getInstance().getFont("Resources/fonts/arial");
        if (sic::debug)
        {
            gui::Label *l = new gui::Label(font);
            l->addStringComponent(new StringComponentString("FPS: "));
            l->addStringComponent(new StringComponentFloat(_fps));
            l->setPosition(-1.0f, 1-l->getSize().y/2.0f);
            l->setScale(0.5);
            addChild(l);
        }

        Label *l = new Label(font);
        l->addStringComponent(new StringComponentString("Loading..."));
        l->setPosition(-l->getSize().x * 0.5f, 0.0f);
        addChild(l);
        _firstUpdate = true;

    }
    LoadingView::~LoadingView()
    {
    }
    void LoadingView::onRender()
    {
        _firstUpdate = false;
    }
    void LoadingView::onUpdate(float dt)
    {
        if (!_firstUpdate)
        {
            //start game
            if (!_parent->setView("HUDView"))
            {
                // std::cout << "scene not found \n Creating new..." << std::endl;
                _parent->setView(new HUDView(_manager, _fps));
            }
            ChangeGameStateEvent event(ChangeGameStateEvent::RunningState);
            _manager->execute(event);
        }
    }
    void LoadingView::initiate()
    {
        _firstUpdate = true;
        cursorModeChangeEvent cEvent(GLFW_CURSOR_NORMAL);
        _manager->execute(cEvent);

    }
}
