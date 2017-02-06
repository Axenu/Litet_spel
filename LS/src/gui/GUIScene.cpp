#include "gui/GUIScene.h"

namespace gui
{
    Scene::Scene() : Element()
    {
        _size = glm::vec2(2,2);
        // _position.x = -1;
        // _position.y = -1;
        // std::cout << "gui scene constructor" << std::endl;
    }
    Scene::~Scene()
    {
        // std::cout << "gui scene destructor" << std::endl;
    }
    void Scene::onRender()
    {

    }
    void Scene::onUpdate(float dt)
    {

    }
    std::string Scene::getName()
    {
        return _name;
    }
    void Scene::setParent(gui::Manager *manager)
    {
        _parent = manager;
    }
    void Scene::testClick(glm::vec2& pos, int action)
    {
        for (Element* child : _children)
        {
            child->testClick(pos, action);
        }
    }
}
