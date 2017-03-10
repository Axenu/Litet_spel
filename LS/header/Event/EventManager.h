#pragma once
#include <map>
#include <vector>
#include <iostream>
#include "type_info.h"

class Event
{
protected:
	virtual ~Event() {};
};

class HandlerFunctionBase
{
public:
	virtual ~HandlerFunctionBase() {};
	void exec(const Event& event) {call(event);}

	bool _shouldDelete = false;

private:
	// virtual bool isEqual(const HandlerFunctionBase &b) const = 0;
	virtual void call(const Event&) = 0;
};


template <class T, class EventT>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void (T::*MemberFunc)(EventT&);
	MemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

	void call(const Event& event)
	{
		(_instance->*_function)(static_cast<EventT&>(event));
	}
	bool isEqual(HandlerFunctionBase *b)
	{
		MemberFunctionHandler *m = dynamic_cast<MemberFunctionHandler<T, EventT>*>(b);
		if (m != nullptr)
		{
			if (_instance == m->_instance && _function == m->_function)
			{
				return true;
			}
		}
		return false;
	}
private:
	T* _instance;
	MemberFunc _function;
};

class EventManager
{
public:
    EventManager();
	~EventManager();
	void execute(const Event&);

	template <class T, class EventT>
	void listen(T*, void (T::*memFn)(EventT&));

	template <class T, class EventT>
	void unlisten(T*, void (T::*memFn)(EventT&));

private:
	typedef std::map<TypeInfo, std::vector<HandlerFunctionBase*>> Handlers;
	Handlers _handlers;
};


template <class T, class EventT>
void EventManager::listen(T* obj, void (T::*memFn)(EventT&))
{
	_handlers[TypeInfo(typeid(EventT))].push_back(new MemberFunctionHandler<T, EventT>(obj, memFn));
}

template <class T, class EventT>
void EventManager::unlisten(T* obj, void (T::*memFn)(EventT&))
{
	std::vector<HandlerFunctionBase*> *v = &_handlers[TypeInfo(typeid(EventT))];
	MemberFunctionHandler<T, EventT> temp(obj, memFn);
	for (std::vector<HandlerFunctionBase*>::iterator i = v->begin(); i != v->end();)
	{
		if (temp.isEqual(*i))
		{
			(*i)->_shouldDelete = true;
			i++;
			break;
		}
		else
		{
			i++;
		}
	}
}
