#pragma once

#include <iostream>
#include <vector>
#include "gl/glInclude.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include  "gl/GLFunctions.h"
#include "Element.h"
#include "Rectangle.h"
#include "Label.h"
#include "StringComponents.h"

namespace gui
{

    class ButtonHandlerBase
    {
    public:
    	virtual ~ButtonHandlerBase() {};
    	void exec(int action) {call(action);}

    private:
    	virtual void call(int) = 0;
    };


    template <class T>
    class ButtonHandler : public ButtonHandlerBase
    {
    public:
    	typedef void (T::*MemberFunc)(int);
    	ButtonHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

    	void call(int action)
    	{
    		(_instance->*_function)(action);
    	}

    private:
    	T* _instance;
    	MemberFunc _function;
    };

    class Button : public Element
    {
    private:
        Font* _font;
        Label* _label;
        Rectangle* _rect;

        float _padding;

        gui::ButtonHandlerBase* _callback = nullptr;

    public:

        Button();
		virtual ~Button();

        template <class T>
    	void listen(T* obj, void (T::*memFn)(int))
        {
			// if (_callback)
			// 	delete _callback;
            _callback = new gui::ButtonHandler<T>(obj, memFn);
        }
        void execute(int action);

        void onRender();
        void onUpdate(float dt);

        virtual void cursorDidEnter();
        virtual void cursorDidExit();
        bool handleClick(int action);

        void addStringComponent(StringComponent* sc);

    };

}
