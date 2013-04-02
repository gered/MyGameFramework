#include "../framework/debug.h"

#include "eventmanager.h"
#include "event.h"
#include "eventlistener.h"

EventManager::EventManager()
{
	m_activeQueue = 0;
}

EventManager::~EventManager()
{
}

bool EventManager::AddListener(EventListener *listener, EVENT_TYPE type)
{
	// TODO: validate type

	EventListenerMap::iterator listenerItor = m_registry.find(type);
	if (listenerItor == m_registry.end())
	{
		// need to register this listener for the given type
		EventListenerMapIRes result = m_registry.insert(EventListenerMapEnt(type, EventListenerTable()));
		ASSERT(result.second != false);
		ASSERT(result.first != m_registry.end());

		listenerItor = result.first;
	}

	// prevent duplicate listeners from being registered
	EventListenerTable &table = (*listenerItor).second;
	for (EventListenerTable::iterator itor = table.begin(); itor != table.end(); ++itor)
	{
		ASSERT(*itor != listener);
	}

	table.push_back(listener);

	// also update the list of currently registered event types
	m_typeList.insert(type);

	return true;
}

bool EventManager::RemoveListener(EventListener *listener, EVENT_TYPE type)
{
	bool result = false;

	// TODO: validate type

	// get the list of listeners for the given event type
	EventListenerMap::iterator itor = m_registry.find(type);
	if (itor == m_registry.end())
		return false;

	// check this list for the specified listener
	EventListenerTable &table = itor->second;
	for (EventListenerTable::iterator j = table.begin(); j != table.end(); ++j)
	{
		if (*j == listener)
		{
			// found the listener
			table.erase(j);
			result = true;

			// if there are no more listeners for this type, remove the type from the list of registered event types
			if (table.size() == 0)
				m_typeList.erase(type);

			// don't need to worry about duplicate listeners since AddListener() stops them from being added...
			break;
		}
	}

	return result;
}

bool EventManager::Trigger(const Event *event) const
{
	// TODO: validate type

	// trigger events for wildcard listeners first
	EventListenerMap::const_iterator wildcardItor = m_registry.find(EVENT_TYPE_WILDCARD);
	if (wildcardItor != m_registry.end())
	{
		const EventListenerTable &table = wildcardItor->second;
		for (EventListenerTable::const_iterator i = table.begin(); i != table.end(); ++i)
		{
			EventListener *listener = *i;
			listener->Handle(event);
		}
	}

	// find the listener list for the event type
	EventListenerMap::const_iterator itor = m_registry.find(event->GetTypeOf());
	if (itor == m_registry.end())
		return false;

	bool result = false;

	// trigger the event in each listener
	const EventListenerTable &table = itor->second;
	for (EventListenerTable::const_iterator i = table.begin(); i != table.end(); ++i)
	{
		EventListener *listener = *i;
		if (listener->Handle(event))
		{
			// only return true if a listener signals they handled the event
			result = true;
		}
	}

	return result;
}

bool EventManager::Queue(const Event *event)
{
	ASSERT(m_activeQueue >= 0);
	ASSERT(m_activeQueue < NUM_EVENT_QUEUES);

	// TODO: validate type

	EventListenerMap::const_iterator itor = m_registry.find(event->GetTypeOf());
	if (itor == m_registry.end())
	{
		// no specific listener for just this event's type... try a wildcard listener next
		EventListenerMap::const_iterator wildcardItor = m_registry.find(EVENT_TYPE_WILDCARD);
		if (wildcardItor == m_registry.end())
		{
			// no wildcard listener either... useless event!
			return false;
		}
	}

	m_queues[m_activeQueue].push_back(event);
	return true;
}

bool EventManager::Abort(EVENT_TYPE type, bool allOfType)
{
	ASSERT(m_activeQueue >= 0);
	ASSERT(m_activeQueue < NUM_EVENT_QUEUES);

	// TODO: validate type

	EventListenerMap::iterator itor = m_registry.find(type);
	if (itor == m_registry.end())
		return false;               // no listeners for this type

	bool result = false;

	EventQueue &queue = m_queues[m_activeQueue];
	for (EventQueue::iterator i = queue.begin(); i != queue.end(); ++i)
	{
		const Event *event = *i;
		if (event->GetTypeOf() == type)
		{
			// found a match, remove it from the queue
			i = queue.erase(i);
			result = true;

			if (!allOfType)
				break;
		}
		else
			++i;
	}

	return result;
}

bool EventManager::ProcessQueue()
{
	EventListenerMap::const_iterator wildcardItor = m_registry.find(EVENT_TYPE_WILDCARD);

	// swap active queues and empty the new queue
	uint queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % NUM_EVENT_QUEUES;
	m_queues[m_activeQueue].clear();

	// process the queue!
	EventQueue &queue = m_queues[queueToProcess];
	while (queue.size() > 0)
	{
		// pop the next event off the queue
		const Event *event = queue.front();
		queue.pop_front();

		EVENT_TYPE type = event->GetTypeOf();

		// process wildcard listeners first (if any)
		if (wildcardItor != m_registry.end())
		{
			const EventListenerTable &table = wildcardItor->second;
			for (EventListenerTable::const_iterator i = table.begin(); i != table.end(); ++i)
			{
				EventListener *listener = *i;
				listener->Handle(event);
			}
		}

		EventListenerMap::const_iterator listenerItor = m_registry.find(type);
		if (listenerItor != m_registry.end())
		{
			const EventListenerTable &table = listenerItor->second;
			for (EventListenerTable::const_iterator i = table.begin(); i != table.end(); ++i)
			{
				EventListener *listener = *i;
				if (listener->Handle(event))
					break;                      // don't let other listeners handle the event if this one signals it handled it
			}
		}

		SAFE_DELETE(event);
	}

	// if there are any events left in the queue, push them onto the active queue
	if (queue.size() != 0)
	{
		while (queue.size() > 0)
		{
			// to preserve sequencing, go bottom-up on the remainder of the queue that was processed
			// inserting them at the head of the active queue
			const Event *event = queue.back();
			queue.pop_back();
			m_queues[m_activeQueue].push_front(event);
		}

		return false;
	}
	else
		return true;
}

EventListenerList EventManager::GetListenerList(EVENT_TYPE type) const
{
	EventListenerMap::const_iterator listenerItor = m_registry.find(type);
	if (listenerItor == m_registry.end())
		return EventListenerList();              // no registered listeners for this type

	const EventListenerTable &table = listenerItor->second;
	if (table.size() == 0)
		return EventListenerList();              // no registered listeners for this type

	EventListenerList result;
	result.reserve(table.size());

	for (EventListenerTable::const_iterator i = table.begin(); i != table.end(); ++i)
		result.push_back(*i);

	return result;
}

EventTypeList EventManager::GetTypeList() const
{
	if (m_typeList.size() == 0)
		return EventTypeList();                  // no types have been registered

	EventTypeList result;
	result.reserve(m_typeList.size());

	for (EventTypeSet::const_iterator i = m_typeList.begin(); i != m_typeList.end(); ++i)
		result.push_back(*i);

	return result;
}
