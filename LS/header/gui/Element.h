#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace gui
{

    class Element
    {
    protected:
        glm::vec3 _position;
        glm::vec2 _size;
        //GL stuff
        int _vertexCount;
        int _indexCount;
    public:

        Element();
        ~Element();

        virtual void render() = 0;

        virtual void update(float dt) = 0;

        //getters
        glm::vec3& getPosition();
        glm::vec2& getSize();

        //setters
        void setPosition(glm::vec2 &pos);
        void setPosition(float x, float y);
        void setSize(glm::vec2 &size);
        void setSize(float width, float height);

    };

}
