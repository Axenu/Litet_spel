#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <map>

#define TEMPLATE template <typename Class>

class EventHandlerBase {
public:
    // virtual void execute() = 0;
    virtual void execute(int keyID, int action) = 0;
    virtual void execute(double keyID, double action) = 0;
    // virtual void execute(int keyID, int action) = 0;
};

// Event Handler Class : Handles Callback
template <typename Class, typename Param1, typename Param2>
class EventHandler : public EventHandlerBase{
    // Defining type for function pointer
    typedef void (Class::*_fptr)(Param1 p1, Param2 p2);
    // typedef void (Class::*_movefptr)(double x, double y);



public:
    // Object of the Listener
    Class *_object;
    // Function for callback
    _fptr _function;
    // _movefptr _mfunction;

    EventHandler(Class *obj, _fptr func) {
        _object = obj;
        _function = func;
    }
    // EventHandler(_movefptr func, Class *obj) {
    //     _object = obj;
    //     _mfunction = func;
    // }
    void execute(int p1, int p2) {
        (_object->*_function)(p1, p2);
    }
    void execute(double p1, double p2) {
        (_object->*_function)(p1, p2);
    }
};

// Class to create a event
class Event {
    // To store all listeners of the event
    typedef std::map<int, EventHandlerBase*> EventHandlerMap;
    EventHandlerMap handlers;
    int count;
public:

    template <typename Class, typename Param1, typename Param2>
    void addListener(Class *obj, void (Class::*func)(Param1 keyID, Param2 action)) {
        handlers[count] = new EventHandler<Class, Param1, Param2>(obj, func);
        count++;
    }

    template <typename Param1, typename Param2>
    void execute(Param1 p1, Param2 p2)
    {
        for (EventHandlerMap::iterator it = handlers.begin(); it != handlers.end(); ++it) {
            it->second->execute(p1, p2);
        }
    }

};

class InputManager
{
    struct EventType {
        Event *event;
        std::string name;
    };

    std::vector<EventType> _events;

    static InputManager *_Instance;

    InputManager(){};
public:
    static InputManager* Instance() {
        if (!_Instance) {
            _Instance = new InputManager();
        }
        return _Instance;
    }

    template <typename Class, typename Param1, typename Param2>
    void subscribe(std::string name, Class *obj, void (Class::*func)(Param1 keyID, Param2 action)) {
        bool found = false;
        for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
            EventType e = *it;
            if (e.name.compare(name) == 0) {
                e.event->addListener<Class, Param1, Param2>(obj, func);
                found = true;
                break;
            }
        }
        if (found == false)
        {
            EventType e;
            e.event = new Event();
            e.name = name;
            _events.push_back(e);
        }
    }
    // template <typename Class>
    // void subscribeMouse(std::string name, Class *obj, void (Class::*func)(double x, double y)) {
    //     bool found = false;
    //     for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
    //         EventType e = *it;
    //         if (e.name.compare(name) == 0) {
    //             e.event->addMouseListener(obj, func);
    //             found = true;
    //             break;
    //         }
    //     }
    //     if (found == false)
    //     {
    //         EventType e;
    //         e.event = new Event();
    //         e.name = name;
    //         _events.push_back(e);
    //     }
    // }
    template <typename Param1, typename Param2>
    void execute(std::string name, Param1 p1, Param2 p2) {
        for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
            EventType e = *it;
            if (e.name.compare(name) == 0) {
                e.event->execute(p1, p2);
            }
        }
    }
    // void execute(std::string name, double x, double y) {
    //     for (std::vector<EventType>::iterator it = _events.begin(); it != _events.end(); ++it) {
    //         EventType e = *it;
    //         if (e.name.compare(name) == 0) {
    //             e.event->execute(x, y);
    //         }
    //     }
    // }
};
