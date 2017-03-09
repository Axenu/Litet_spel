#include "Event/eventManager.h"

//better use boost::ptr_map instead of std::map
EventManager::EventManager()
{

}

EventManager::~EventManager()
{
	Handlers::iterator it = _handlers.begin();
	while(it != _handlers.end())
	{
        for (std::vector<HandlerFunctionBase*>::iterator i = it->second.begin(); i != it->second.end(); ++i)
		{
            delete *i;
        }
		++it;
	}
	_handlers.clear();
}

void EventManager::execute(const Event& event)
{
	Handlers::iterator it = _handlers.find(TypeInfo(typeid(event)));
	if(it != _handlers.end())
	{
        for (std::vector<HandlerFunctionBase*>::iterator i = it->second.begin(); i != it->second.end();)
		{
			if ((*i)->_shouldDelete)
			{
				delete *i;
				i = it->second.erase(i);
			}
			else
			{
				(*i)->exec(event);
				i++;
			}
        }
	}
}
