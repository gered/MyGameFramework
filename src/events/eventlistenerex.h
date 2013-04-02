#ifndef __EVENTS_EVENTLISTENEREX_H_INCLUDED__
#define __EVENTS_EVENTLISTENEREX_H_INCLUDED__

#include "../framework/common.h"
#include "eventlistener.h"
#include "eventmanager.h"

struct Event;

class EventListenerEx : public EventListener
{
public:
	EventListenerEx(EventManager *eventManager);
	virtual ~EventListenerEx();

	template<class T> bool ListenFor();
	template<class T> bool StopListeningFor();
	bool ListenFor(EVENT_TYPE type);
	bool StopListeningFor(EVENT_TYPE type);

	virtual bool Handle(const Event *event) = 0;

	EventManager* GetEventManager() const                  { return m_eventManager; }

private:
	EventManager *m_eventManager;
};

inline EventListenerEx::EventListenerEx(EventManager *eventManager)
{
	m_eventManager = eventManager;
}

inline EventListenerEx::~EventListenerEx()
{
}

template<class T>
inline bool EventListenerEx::ListenFor()
{
	return m_eventManager->AddListener<T>(this);
}

template<class T>
inline bool EventListenerEx::StopListeningFor()
{
	return m_eventManager->RemoveListener<T>(this);
}

inline bool EventListenerEx::ListenFor(EVENT_TYPE type)
{
	return m_eventManager->AddListener(this, type);
}

inline bool EventListenerEx::StopListeningFor(EVENT_TYPE type)
{
	return m_eventManager->RemoveListener(this, type);
}

#endif

