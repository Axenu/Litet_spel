#include "gui/GUIScene.h"

namespace gui
{
    Scene::Scene() : Element()
    {
        _size = glm::vec2(2,2);
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
}
