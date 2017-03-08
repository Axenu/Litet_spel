#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <algorithm>

namespace gui
{

#define PALLETBACKGROUND glm::vec4(0.20703125f, 0.29296875f, 0.28125f, 1.0f)
#define PALLETPRIMARY glm::vec4(0.4140625f, 0.57421875f, 0.5234375f, 1.0f)
#define PALLETHIGHLIGHT glm::vec4(0.5859375f, 0.734375f, 0.64453125f, 1.0f)

    class Element
    {
    protected:
        glm::vec3 _position;
        glm::vec2 _scale;
        glm::vec2 _size;
        glm::mat3 _modelMatrix;
        std::vector<Element*> _children;
        Element* _parent = nullptr;
        bool _isActive = true;
        bool _isReactive = false;
        //GL stuff
        int _vertexCount;
        int _indexCount;
    public:

        Element();
        virtual ~Element();

        void render(float dt);
        virtual void onRender(float dt) = 0;
        void update(float dt);
        virtual void onUpdate(float dt) = 0;

        void addChild(Element* child);

        Element *checkCollision(glm::vec2 &pos);
        virtual void cursorDidEnter(glm::vec2 pos) {}
        virtual void cursorDidExit(glm::vec2 pos) {}
        virtual void cursorMovedInside(glm::vec2 pos) {}
        virtual bool handleClick(int action, glm::vec2 pos);


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
        void setReactive(bool react);

        void activate();
        void deactivate();

    };

}
