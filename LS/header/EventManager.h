#pragma once
#include <map>
#include <vector>
#include "type_info.h"

class Event
{
protected:
	virtual ~Event() {};
};

class Explosion : public Event
{
public:
	Explosion(int pos) : _pos(pos), _damage(60) {};

	int getPosition()	const	{return _pos;}
	int getDamage()		const	{return _damage;}

private:
	int _pos, _damage;
};

class HandlerFunctionBase
{
public:
	virtual ~HandlerFunctionBase() {};
	void exec(const Event* event) {call(event);}

private:
	virtual void call(const Event*) = 0;
};


template <class T, class EventT>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void (T::*MemberFunc)(EventT*);
	MemberFunctionHandler(T* instance, MemberFunc memFn) : _instance(instance), _function(memFn) {};

	void call(const Event* event)
	{
		(_instance->*_function)(static_cast<EventT*>(event));
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
	void handleEvent(const Event*);

	template <class T, class EventT>
	void registerEventFunc(T*, void (T::*memFn)(EventT*));

private:
	typedef std::map<TypeInfo, std::vector<HandlerFunctionBase*>> Handlers;
	Handlers _handlers;
};


template <class T, class EventT>
void EventManager::registerEventFunc(T* obj, void (T::*memFn)(EventT*))
{
	_handlers[TypeInfo(typeid(EventT))].push_back(new MemberFunctionHandler<T, EventT>(obj, memFn));
}
