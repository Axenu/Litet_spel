#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace gui
{

    class Element
    {
    protected:
        glm::vec3 _position;
        glm::vec2 _scale;
        glm::vec2 _size;
        glm::mat3 _modelMatrix;
        std::vector<Element*> _children;
        Element* _parent = nullptr;
        //GL stuff
        int _vertexCount;
        int _indexCount;
    public:

        Element();
        virtual ~Element();

        void render();
        virtual void onRender() = 0;
        void update(float dt);
        virtual void onUpdate(float dt) = 0;

        void addChild(Element* child);

        void testClick(glm::vec2& pos, int action);
        virtual bool handleClick(int action);


        //getters
        glm::vec3& getPosition();
        glm::vec2& getSize();
        glm::vec2& getScale();

        //setters
        void setPosition(glm::vec2 &pos);
        void setPosition(float x, float y);
        void setZ(float Z); //set Z value between 0 and 99. 99 is on top. default 50.
        void setSize(glm::vec2 &size);
        void setSize(float width, float height);
        void setScale(glm::vec2 &scale);
        void setScale(float x, float y);
        void setScale(float s);

    };

}
