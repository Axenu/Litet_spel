#pragma once

#include <iostream>
#include "node.h"
#include "camera.h"
#include "InputManager.h"
#include <glm/glm.hpp>

class Character : public Node
{
public:
    void setCamera(Camera *camera);

    void onUpdate(float dt);
    void onRender();

    void moveCharacter(int buttonID, int action);
    void moveMouse(double x, double y);

    // Character(InputManager *manager);
    Character();
    ~Character();
private:
    Camera* _camera;
    // InputManager* _manager;
    glm::vec3 _velocity;
};
