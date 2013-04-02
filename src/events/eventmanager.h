#ifndef __EVENTS_EVENTMANAGER_H_INCLUDED__
#define __EVENTS_EVENTMANAGER_H_INCLUDED__

#include "../framework/common.h"

#include "event.h"

#include <stl/list.h>
#include <stl/set.h>
#include <stl/vector.h>
#include <stl/map.h>

class EventListener;

typedef stl::vector<EventListener*> EventListenerList;
typedef stl::vector<EVENT_TYPE> EventTypeList;

const int NUM_EVENT_QUEUES = 2;

class EventManager
{
public:
	EventManager();
	virtual ~EventManager();

	template<class T> BOOL AddListener(EventListener *listener);
	template<class T> BOOL RemoveListener(EventListener *listener);
	BOOL AddListener(EventListener *listener, EVENT_TYPE type);
	BOOL RemoveListener(EventListener *listener, EVENT_TYPE type);

	BOOL Trigger(const Event *event) const;
	BOOL Queue(const Event *event);
	template<class T> BOOL Abort(BOOL allOfType = FALSE);
	BOOL Abort(EVENT_TYPE type, BOOL allOfType = FALSE);

	BOOL ProcessQueue();

	template<class T> EventListenerList GetListenerList() const;
	EventListenerList GetListenerList(EVENT_TYPE type) const;
	EventTypeList GetTypeList() const;

private:
	typedef stl::set<EVENT_TYPE> EventTypeSet;
	typedef stl::pair<EventTypeSet::iterator, BOOL> EventTypeSetIRes;
	typedef stl::list<EventListener*> EventListenerTable;
	typedef stl::map<EVENT_TYPE, EventListenerTable> EventListenerMap;
	typedef stl::pair<EVENT_TYPE, EventListenerTable> EventListenerMapEnt;
	typedef stl::pair<EventListenerMap::iterator, BOOL> EventListenerMapIRes;
	typedef stl::list<const Event*> EventQueue;

	EventTypeSet m_typeList;
	EventListenerMap m_registry;
	EventQueue m_queues[NUM_EVENT_QUEUES];
	uint m_activeQueue;
};

template<class T>
BOOL EventManager::AddListener(EventListener *listener)
{
	return AddListener(listener, T::GetType());
}

template<class T>
BOOL EventManager::RemoveListener(EventListener *listener)
{
	return RemoveListener(listener, T::GetType());
}

template<class T>
BOOL EventManager::Abort(BOOL allOfType)
{
	return Abort(T::GetType(), allOfType);
}

template<class T>
EventListenerList EventManager::GetListenerList() const
{
	return GetListenerList(T::GetType());
}

#endif

